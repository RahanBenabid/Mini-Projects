#!/usr/bin/env python3

import os
import io
import base64
from pikepdf import Pdf, AttachedFileSpec, Name, Dictionary, Array
from zipfile import ZipFile

def getBytesFromPDF():
  with open("./test.pdf", "rb") as myPDF:
    bytes_data = myPDF.read()
    print(bytes_data)
    return bytes_data
  
def embedPdf(host, pdf_list):
  #	print(host, pdf_list[0])
  host_pdf = Pdf.open(host)
  
  for (idx, pdf_file) in enumerate(pdf_list):
    file_bytes = pdf_list[idx].read()
    host_pdf.attachments[pdf_file.filename] = file_bytes
    
  output = io.BytesIO()
  host_pdf.save(output)
  output.seek(0)
  
  return output

def extractPdf(pdf):
  
  myPdf = Pdf.open(pdf)
  attachments = myPdf.attachments
  myFiles = []
  
  zip_buffer = io.BytesIO()
  
  with ZipFile(zip_buffer, 'w') as archive:
    for attachment in attachments:
      pdfContent = myPdf.attachments[attachment].get_file()
      pdfBytes = pdfContent.read_bytes()
      archive.writestr(attachment, pdfBytes)
           
  zip_buffer.seek(0)
  
  return zip_buffer

# link https://pikepdf.readthedocs.io/en/latest/topics/attachments.html