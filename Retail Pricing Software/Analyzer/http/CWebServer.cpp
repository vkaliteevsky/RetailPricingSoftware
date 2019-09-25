/*
 * CWebServer.cpp
 *
 *  Created on: Jan 19, 2019
 *      Author: kraychik
 */

#include "CWebServer.h"
#include "../tasks/CTaskManager.h"

CWebServer::CWebServer() {
	// TODO Auto-generated constructor stub
	this->start ();
}

CWebServer::~CWebServer() {
	// TODO Auto-generated destructor stub
}

string CWebServer::syncProject(int projectID, bool isSyncRoles) {
	//return this->processing->syncProject(projectID, isSyncRoles);
	return string("");
}

void CWebServer::dump_request_cb(struct evhttp_request* req, void* arg) {
	const char *cmdtype;
	struct evkeyvalq *headers;
	struct evkeyval *header;
	struct evbuffer *buf;

	switch (evhttp_request_get_command(req)) {
	case EVHTTP_REQ_GET: cmdtype = "GET"; break;
	case EVHTTP_REQ_POST: cmdtype = "POST"; break;
	case EVHTTP_REQ_HEAD: cmdtype = "HEAD"; break;
	case EVHTTP_REQ_PUT: cmdtype = "PUT"; break;
	case EVHTTP_REQ_DELETE: cmdtype = "DELETE"; break;
	case EVHTTP_REQ_OPTIONS: cmdtype = "OPTIONS"; break;
	case EVHTTP_REQ_TRACE: cmdtype = "TRACE"; break;
	case EVHTTP_REQ_CONNECT: cmdtype = "CONNECT"; break;
	case EVHTTP_REQ_PATCH: cmdtype = "PATCH"; break;
	default: cmdtype = "unknown"; break;
	}

	printf("Received a %s request for %s\nHeaders:\n", cmdtype, evhttp_request_get_uri(req));

	headers = evhttp_request_get_input_headers(req);
	for (header = headers->tqh_first; header;
		header = header->next.tqe_next) {
		printf("  %s: %s\n", header->key, header->value);
	}

	buf = evhttp_request_get_input_buffer(req);
	puts("Input data: <<<");
	while (evbuffer_get_length(buf)) {
		int n;
		char cbuf[128];
		n = evbuffer_remove(buf, cbuf, sizeof(cbuf));
		if (n > 0)
			(void) fwrite(cbuf, 1, n, stdout);
	}
	puts(">>>");

	evhttp_send_reply(req, 200, "OK", NULL);

}

void CWebServer::send_document_cb(struct evhttp_request* req, void* arg) {
	const char *uri = evhttp_request_get_uri(req);
		if (evhttp_request_get_command(req) != EVHTTP_REQ_GET) {
			dump_request_cb(req, arg);
			return;
		}

		struct evhttp_uri *decoded = evhttp_uri_parse(uri);
		if (!decoded) {
			printf("It's not a good URI. Sending BADREQUEST\n");
			evhttp_send_error(req, HTTP_BADREQUEST, 0);
			return;
		}

		const char *path = evhttp_uri_get_path(decoded);
		if (!path) {
			path = "/";
		}

		if (strstr(path, "..")) {
			return;
		}

		auto newTask = CTaskManager::CreateTask(uri);
		std::string strResponse;
		if (newTask != nullptr) {
			strResponse = FormResponseString(newTask);
			//cout << newTask->ToString() << endl;
		} else {
			strResponse = CJSONCreator::CreateClassicResponse(-1, nullptr, "Bad Request", false);
		}

		CWebServer *webServer = (CWebServer *)arg;
		struct evbuffer *evb = evbuffer_new();

		//strResponse = uri;
		evbuffer_add_printf(evb, "%s", strResponse.c_str ());

		evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", "application/json");
		evhttp_send_reply(req, 200, "OK", evb);

		if (decoded) {
			evhttp_uri_free(decoded);
		}

		if (evb) {
			evbuffer_free(evb);
		}
}

void CWebServer::run() {
	char uri_root[512];
	struct event_base *base;
		struct evhttp *http;
		struct evhttp_bound_socket *handle;

		ev_uint16_t port = 7553;

		if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
			throw "wrong SIG_ERR";
		}

		base = event_base_new();
		if (!base) {
			fprintf(stderr, "Couldn't create an event_base: exiting\n");
			return;
		}

		/* Create a new evhttp object to handle requests. */
		http = evhttp_new(base);
		if (!http) {
			fprintf(stderr, "couldn't create evhttp. Exiting.\n");
			return;
		}

		/* We want to accept arbitrary requests, so we need to set a "generic"
		 * cb.  We can also add callbacks for specific paths. */
		evhttp_set_gencb(http, send_document_cb, this);

		/* Now we tell the evhttp what port to listen on */
		handle = evhttp_bind_socket_with_handle(http, "0.0.0.0", port);
		if (!handle) {
			fprintf(stderr, "couldn't bind to port %d. Exiting.\n", (int)port);
			return;
		}

		{
			/* Extract and display the address we're listening on. */
			struct sockaddr_storage ss;
			evutil_socket_t fd;
			ev_socklen_t socklen = sizeof(ss);
			char addrbuf[128];
			void *inaddr;
			const char *addr;
			int got_port = -1;
			fd = evhttp_bound_socket_get_fd(handle);
			memset(&ss, 0, sizeof(ss));
			if (getsockname(fd, (struct sockaddr *)&ss, &socklen)) {
				perror("getsockname() failed");
				return;
			}
			if (ss.ss_family == AF_INET) {
				got_port = ntohs(((struct sockaddr_in*)&ss)->sin_port);
				inaddr = &((struct sockaddr_in*)&ss)->sin_addr;
			} else if (ss.ss_family == AF_INET6) {
				got_port = ntohs(((struct sockaddr_in6*)&ss)->sin6_port);
				inaddr = &((struct sockaddr_in6*)&ss)->sin6_addr;
			} else {
				fprintf(stderr, "Weird address family %d\n", ss.ss_family);
				return;
			}
			addr = evutil_inet_ntop(ss.ss_family, inaddr, addrbuf, sizeof(addrbuf));
			if (addr) {
				printf("Listening on %s:%d\n", addr, got_port);
				evutil_snprintf(uri_root, sizeof(uri_root), "http://%s:%d",addr,got_port);
			} else {
				fprintf(stderr, "evutil_inet_ntop failed\n");
				return;
			}
		}

		event_base_dispatch(base);
}

string CWebServer::FormResponseString(shared_ptr<const CBaseTask> baseTask) {
	string strResponse;
	int calcId = baseTask->calcId;
	bool isSuccess = true;
	if (shared_ptr<const CGetResultTask> task = dynamic_pointer_cast<const CGetResultTask>(baseTask)) {
		// случай CGetResultTask
		const char *response;
		CResultManager *resultManager = CResultManager::GetInstance();
		auto calcResult = resultManager->GetResult(calcId);
		//strResponse = "{ \"calcId\": " + to_string(calcId) + ", ";
		if (calcResult->status == CalcResultStatus::SUCCESS) {
			response = calcResult->resultString.c_str();
			isSuccess = true;
			//strResponse += "\"response\": " + calcResult->resultString + " }";
		} else if (calcResult->status == CalcResultStatus::WAIT) {
			response = "wait"; isSuccess = true;
			//strResponse += "\"response\": wait }";
		} else if (calcResult->status == CalcResultStatus::ERROR) {
			response = "error"; isSuccess = false;
			//strResponse += "\"response\": error }";
		}
		strResponse = CJSONCreator::CreateClassicResponse(calcId, response, nullptr, isSuccess);
	} else {
		strResponse = CJSONCreator::CreateClassicResponse(calcId, nullptr, nullptr, true);
		//return string("Не реализован случай в CWebServer::FormResponseString");
	}
	return strResponse;
}
