# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: https://docs.scrapy.org/en/latest/topics/item-pipeline.html

# useful for process the scraped data from the web (cleaning validation, storage...)

# useful for handling different item types with a single interface
from itemadapter import ItemAdapter
import pymongo
import hashlib
from scrapy.exceptions import DropItem

class MongoPipeline:
    # specify the name of the collection where you want to store the item
    COLLECTION_NAME = "books"
    
    # initializs the pipeline, the infos are accessible thanks to the .fromcrawler() class method
    def __init__(self, mongo_uri, mongo_db):
        self.mongo_uri = mongo_uri
        self.mongo_db = mongo_db or "default_db"
        
    # gives u access to all core scrapy components
    @classmethod
    def from_crawler(cls, crawler):
        return cls(
            mongo_uri=crawler.settings.get("MONGO_URI"),
            mongo_db=crawler.settings.get("MONGO_DATABASE"),
        )
    
    # opens a connection to mongodb when the spider starts
    def open_spider(self, spider):
        self.client = pymongo.MongoClient(self.mongo_uri)
        self.db = self.client[self.mongo_db]
        
    # closes the connection whrn the spider finishes
    def close_spider(self, spider):
        self.client.close()
    
    # inserts each item into the mongodb collection (table)
    # we implement a functionality that avoids storing duplicates in the databse
    def process_item(self, item, spider):
        item_id = self.compute_item_id(item)
        # checks if there is a book with a similar id to the one we just hashed
        if self.db[self.COLLECTION_NAME].find_one({"_id": item_id}):
            raise DropItem(f'Duplicate item found: {item}')
        else:
            # creates a new book in the database, but instead of the default mongodb _id, we assign the id as the hashed url
            item["_id"] = item_id
            self.db[self.COLLECTION_NAME].insert_one(ItemAdapter(item).asdict())
            return item
        
    # you can make the code update each book for example instead of dropiing the table with a test
        
    # creted a hashed value based on the url, the same url will result in the same hash
    def compute_item_id(self, item):
        url = item["url"]
        return hashlib.sha256(url.encode("utf-8")).hexdigest()