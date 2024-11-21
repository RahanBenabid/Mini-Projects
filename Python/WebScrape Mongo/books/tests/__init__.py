import unittest
from scrapy.http import HtmlResponse, Request
from books.items import BooksItem
from books.spiders.book import BookSpider

class BookSpiderTest(unittest.TestCase):
	def setUp(self):
		self.spider = BookSpider()
		self.example_html = """
			Insert the example HTML here
		"""
		
		self.repsonse = HtmlResponse(
			url="https://books.toscrape.com",
			body=self.example_html,
			encoding="utf-8"
		)
	
	def test_parse_scrapes_all_items(self):
		"""Test if the spider scrapes books and pagination links."""
		results = list(self.spider.parse(self.response))
		book_items = [item for item in results if isinstance(item, BooksItem)]
		pagination_request = [
			item for item in results if isinstance(item, Request)
		]
		
		self.assertEqual(len(book_items), 2)
		self.assertEqual(len(pagination_request), 1)
	
	def test_parse_scrapes_correct_book_information(slef):
		"""Test if the spider scrapes the correct infors for each book"""
	
	def test_parse_creates_pagination_request(self):
		"Test if the spider creates a pagination request correctly"
		pass
		
if __name__ == "__main__":
	unittest.main()