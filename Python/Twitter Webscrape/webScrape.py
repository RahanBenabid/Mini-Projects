import sys
sys.path.insert(0, './local_modules/')

import os
import tweepy
from dotenv import load_dotenv
from tweepy import OAuthHandler

# Load environment variables from .env file
load_dotenv()

# Retrieve credentials from environment variables
consumer_key = os.getenv('TWITTER_CONSUMER_KEY')
consumer_secret = os.getenv('TWITTER_CONSUMER_SECRET')
access_token = os.getenv('TWITTER_ACCESS_TOKEN')
access_secret = os.getenv('TWITTER_ACCESS_SECRET')

# Authenticate to Twitter
auth = OAuthHandler(consumer_key, consumer_secret)
auth.set_access_token(access_token, access_secret)

api = tweepy.API(auth)

# Verify credentials
try:
    api.verify_credentials()
    print("Authentication OK")
except Exception as e:
    print("Error during authentication:", e)
    
# Path to the image you want to upload
image_path = './images/twitter_sample.jpg'
status_message = 'Here is my tweet with an image!'

try:
    # Upload the image
    media = api.media_upload(image_path)
    
    # Post the tweet with the uploaded media
    api.update_status(status=status_message, media_ids=[media.media_id])
    
    print("Tweet posted successfully")
except tweepy.TweepyException as e:
    print("Error posting the tweet:", e)