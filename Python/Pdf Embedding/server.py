#!/usr/bin/env python3

from flask import Flask
from pdfHandler import getBytesFromPDF

# local function

app = Flask(__name__)

@app.route("/")
def servePdf():
  myBytes = getBytesFromPDF()
  return myBytes

app.route("/getPdf", METH)

if __name__ == "__main__":
  print("Server is running @ http://127.0.0.1:5000...")
  app.run(debug=True)
