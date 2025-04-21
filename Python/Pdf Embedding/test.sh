#!/usr/bin/env bash

set -euo pipefail

curl -X POST http://127.0.0.1:5000/upload \
  -F 'host=@/Users/RahanBen/Desktop/test/test.pdf' \
  -F 'pdfs=@//Users/RahanBen/Desktop/test/pdf1.pdf' \
  -F 'pdfs=@//Users/RahanBen/Desktop/test/pdf2.pdf'
  
# curl -F 'files[]=@/path/to/fileX' -F 'files[]=@/path/to/fileY' ... http://localhost/upload