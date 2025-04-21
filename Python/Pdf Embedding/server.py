#!/usr/bin/env python3

import os
from flask import Flask, request, send_file
from werkzeug.utils import secure_filename

# local function
from pdfHandler import getBytesFromPDF, embedPdf

app = Flask(__name__)

@app.route("/")
def servePdf():
  myBytes = getBytesFromPDF()
  return myBytes


@app.route("/upload", methods=['POST'])
def uploadPdf():
  if request.method == 'POST':
    if 'host' not in request.files:
      return 'No file part', 400
    host = request.files['host']
    filename = secure_filename(host.filename)
    
    # save the uploaded file to a directory
    host.save(os.path.join("/Users/RahanBen/Downloads/test/pythonPdf/", filename))
    
    # print the other Pdfs that should be inside:
    pdf_files = request.files.getlist('pdfs')
    if not pdf_files:
      return 'No pdf to embed into the host were sent', 400
    
    embedPdf(host=host, pdf_list=pdf_files)
    print("operation was successful")
        
    
    
    return 'File uploaded successfully!', 200
  else:
    return 'Invalid request method!', 400



if __name__ == "__main__":
  print("Server is running @ http://127.0.0.1:5000...")
  app.run(debug=True)
