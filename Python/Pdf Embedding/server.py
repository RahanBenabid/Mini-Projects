#!/usr/bin/env python3

import os
import io
from flask import Flask, request, send_file
from flask_restful import Api, Resource
from werkzeug.utils import secure_filename
from flasgger import Swagger, swag_from

# local function
from pdfHandler import getBytesFromPDF, embedPdf, extractPdf

app = Flask(__name__)
swagger = Swagger(app)

@app.route("/create_embedded_pdf", methods=['POST'])
def embed_pdf():
  """
  Embed PDFs into a host PDF
  ---
  consumes:
    - multipart/form-data
  parameters:
    - in: formData
      name: host
      type: file
      required: true
      description: The host PDF file
    - in: formData
      name: pdfs
      type: file
      required: true
      description: The PDFs to embed into the host
      collectionFormat: multi
  responses:
    200:
      description: Embedded PDF returned
      content:
        application/pdf:
          schema:
            type: string
            format: binary
    400:
      description: Missing files or wrong request
  """
  if request.method != 'POST':
    return 'Invalid request method!', 400
  
  if 'host' not in request.files:
    return 'No file part', 400
  
  host = request.files['host']
  filename = secure_filename(host.filename)
  
  pdf_files = request.files.getlist('pdfs')
  if not pdf_files:
    return 'No pdf to embed into the host were sent', 400
  
  embeddedPdfByte = embedPdf(host=host, pdf_list=pdf_files)
  print("operation was successful", embeddedPdfByte)
  
  
  return send_file(embeddedPdfByte, as_attachment=True, download_name="embedded.pdf", mimetype='application/pdf')

@app.route("/extract_embedded_pdf", methods=['POST'])
def extract_pdf():
  """
      Extract embedded PDFs from a PDF
      ---
      consumes:
        - multipart/form-data
      parameters:
        - in: formData
          name: pdf
          type: file
          required: true
          description: PDF containing embedded PDFs
      responses:
        200:
          description: ZIP containing extracted PDFs
          content:
            application/zip:
              schema:
                type: string
                format: binary
        400:
          description: No PDF to process
  """
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
  