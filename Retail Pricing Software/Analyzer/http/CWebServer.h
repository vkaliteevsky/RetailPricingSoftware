/*
 * CWebServer.h
 *
 *  Created on: Jan 19, 2019
 *      Author: kraychik
 */

#ifndef HTTP_CWEBSERVER_H_
#define HTTP_CWEBSERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <memory>
#include <map>
#include <typeinfo>

#include <sys/types.h>
#include <sys/stat.h>

#include <sys/stat.h>
#include <sys/socket.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>

#include "Runnable.h"
#include "../processing/CResultManager.h"
#include "../tasks/CBaseTask.h"
#include "CJSONCreator.h"
//#include "CProcessing.h"


#ifdef EVENT__HAVE_NETINET_IN_H
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#endif

#ifdef EVENT__HAVE_NETINET_IN_H
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#endif

using namespace std;

class CWebServer : public Runnable {
	public:
		CWebServer();
		virtual ~CWebServer();
		string syncProject (int projectID, bool isSyncRoles);
		static void dump_request_cb(struct evhttp_request *req, void *arg);
		static void send_document_cb(struct evhttp_request *req, void *arg);
		static string FormResponseString(shared_ptr<const CBaseTask> baseTask);
		//static string MakeJSON(int calcId, const char* response, const char* error, bool isSuccess);
	private:
		void run ();
public:
		static void parseUrl(const char *url, map<string, string> & keys, string & mode, int & projectId);
		//CProcessing *processing = NULL;
};

#endif /* HTTP_CWEBSERVER_H_ */
