import requests
import sys

while True:
    requests.get("http://"+sys.argv[1])
