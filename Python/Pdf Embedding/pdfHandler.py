#!/usr/bin/env python3

import pymupdf
import base64


def getBytesFromPDF():
	with open("./test.pdf", "rb") as myPDF:
		bytes_data = myPDF.read()
		print(bytes_data)
		return bytes_data
	
#with open("test.pdf", "rb") as testPDF:
# encodedPDFString = base64.b64decode(testPDF.read())