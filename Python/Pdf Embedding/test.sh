#!/usr/bin/env bash

set -euo pipefail

echo $PWD
#curl -X POST http://127.0.0.1:5000/create_embedded_pdf \
# -F 'host=@/Users/RahanBen/Desktop/test/test.pdf' \
# -F 'pdfs=@/Users/RahanBen/Desktop/test/pdf1.pdf' \
# -F 'pdfs=@/Users/RahanBen/Desktop/test/pdf2.pdf' \
# --output "./result.pdf"
  
curl -X POST http://127.0.0.1:5000/extract_embedded_pdf --output "./myPdfs.zip" \
  -F 'pdf=@/Users/RahanBen/Desktop/test/result.pdf'
  
# curl -F 'files[]=@/path/to/fileX' -F 'files[]=@/path/to/fileY' ... http://localhost/upload