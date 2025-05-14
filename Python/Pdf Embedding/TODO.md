# Implementig hashing

1. For Embedding:
	- Calculate and store hashes of all embedded PDFs
	- You could store these hashes in a custom metadata field of the host PDF
	- Or create a separate "manifest" file that lists all embedded PDFs with their hashes

2. For Extraction:
	- Verify the hash of each extracted PDF against stored values
	- Flag or warn about any inconsistencies

3. Document Signing:
	- Sign the final document after embedding to ensure it hasn't been tampered with
	- This provides an additional layer of security beyond just hashing


1. Hash Verification
	- Purpose: Verify file integrity by ensuring it hasn't been modified
	- Approach: Calculate a hash (like SHA-256) of each PDF before embedding and store this information
	- When extracting: Recalculate hashes and compare them to verify integrity

2. Digital Signatures
	- Purpose: Both verify integrity and authenticate the source of the document
	- More sophisticated than simple hashing - uses public/private key cryptography
	- Can be visible (appears on the document) or invisible

3. PDF Metadata Authentication
	- Store authentication information in the PDF metadata
	- Could include hashes of embedded documents, timestamp, author information