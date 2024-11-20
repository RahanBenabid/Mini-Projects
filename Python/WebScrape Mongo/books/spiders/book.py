import scrapy

from books.items import BooksItem

# we set up parse() to first find all the books, and then iterate over them in the for loop

class BookSpider(scrapy.Spider):
    name = "book"
    allowed_domains = ["books.toscrape.com"]
    start_urls = ["https://books.toscrape.com/"]

    def parse(self, response):
        for book in response.css("article.product_pod"):
            item = BooksItem()
            # we are using relative paths in case the website changes
            item["url"] = book.css("h3 > a::attr(href)").get()
            item["title"] = book.css("h3 > a::attr(title)").get()
            item["price"] = book.css(".price_color::text").get()
            yield item
        
        # we implement some web scrawling, telling scrapy how to access the next page
        # this will handle pagination
        next_page = response.css("li.next > a::attr(href)").get()
        if next_page:
            next_page_url = response.urljoin(next_page)
            self.logger.info(
                f'Navigating to next page with URL {next_page_url}.'
            )
            yield scrapy.Request(url=next_page_url, callback=self.parse)
        # this will be done until the last page is reached
    
    
    # helps for logging
    def log_error(self, failure):
        self.logger.error(repr(failure))