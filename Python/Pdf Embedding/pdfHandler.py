#!/usr/bin/env python3

import base64
from pikepdf import Pdf, AttachedFileSpec, Name, Dictionary, Array
import os



def getBytesFromPDF():
	with open("./test.pdf", "rb") as myPDF:
		bytes_data = myPDF.read()
		print(bytes_data)
		return bytes_data
	
def embedPdf(host, pdf_list):
	#	print(host, pdf_list[0])
	host_pdf = Pdf.open(host)
	#	filespec = AttachedFileSpec.from_filepath(host_pdf, pdf_list[0])
	
	for (idx, pdf_file) in enumerate(pdf_list):
		file_bytes = pdf_list[idx].read()
		host_pdf.attachments[pdf_file.filename] = file_bytes
	
	print("infoooo:", host_pdf.attachments)
	
	host_pdf.save(os.path.join("/Users/RahanBen/Downloads/test/pythonPdf/", "newEmbeddedPdf.pdf"))
	return

def extractPdf(host):
	pass
	
#with open("test.pdf", "rb") as testPDF:
# encodedPDFString = base64.b64decode(testPDF.read())
	
# link https://pikepdf.readthedocs.io/en/latest/topics/attachments.html