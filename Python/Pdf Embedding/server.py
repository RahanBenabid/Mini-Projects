#!/usr/bin/env python3

import os
import io
from flask import Flask, request, send_file
from werkzeug.utils import secure_filename

# local function
from pdfHandler import getBytesFromPDF, embedPdf, extractPdf

app = Flask(__name__)

@app.route("/")
def servePdf():
  myBytes = getBytesFromPDF()
  return myBytes


@app.route("/create_embedded_pdf", methods=['POST'])
def embed_pdf():
  if request.method != 'POST':
    return 'Invalid request method!', 400
  
  if 'host' not in request.files:
    return 'No file part', 400
  host = request.files['host']
  filename = secure_filename(host.filename)
  
  # print the other Pdfs that should be inside:
  pdf_files = request.files.getlist('pdfs')
  if not pdf_files:
    return 'No pdf to embed into the host were sent', 400
  
  embeddedPdfByte = embedPdf(host=host, pdf_list=pdf_files)
  print("operation was successful", embeddedPdfByte)
  
  
  return send_file(embeddedPdfByte, as_attachment=True, download_name="embedded.pdf", mimetype='application/pdf')

@app.route("/extract_embedded_pdf", methods=['POST'])
def extract_pdf():
  if request.method != 'POST':
    return 'Invalid request method!', 400
  
  if 'pdf' not in request.files:
    return 'No Pdf to process', 400
  
  pdf = request.files['pdf']
  
  zippedPdfs = extractPdf(pdf)
  
  return send_file(zippedPdfs, as_attachment=True, download_name="archive.zip", mimetype='application/zip')
  
  


if __name__ == "__main__":
  print("Server is running @ http://127.0.0.1:5000...")
  app.run(debug=True)
