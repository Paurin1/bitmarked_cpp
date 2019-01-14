#!/usr/bin/env python

import sys
import http.client

if len(sys.argv) != 2 and len(sys.argv) != 4:
	sys.exit(1)

httpClient = http.client.HTTPSConnection("www.bitmarket.pl")
outputFile = open("python_json_output", "w")

#get request
if len(sys.argv) == 2:
	httpClient.request("GET", sys.argv[1])
	outputFile.write( httpClient.getresponse().read().decode("utf-8") )

#post request
elif len(sys.argv) == 4:
	headers = {}
	for header in sys.argv[3].split('&'):
		headers[ header.split("=")[0] ] = header.split("=")[1]
	httpClient.request("POST", sys.argv[1], sys.argv[2], headers)
	outputFile.write( httpClient.getresponse().read().decode("utf-8") )

outputFile.close()
