'''
Script to automatically check price of USA grad visa online, texts results using Twilio API

Author: Padraig O Cosgora
Date: 14/01/2020


'''

import bs4, requests, smtplib  # Download the helper library from https://www.twilio.com/docs/python/install
from twilio.rest import Client

# Download page
getPage = requests.get('https://www.usavisa.ie/1-year-graduate-visa/cost')
getPage.raise_for_status()  # if error it will stop the program

# Your Account Sid and Auth Token from twilio.com/console
# DANGER! This is insecure. See http://twil.io/secure
account_sid = '*****************************'
auth_token = '**************************'
client = Client(account_sid, auth_token)

# Parse text for foods
menu = bs4.BeautifulSoup(getPage.text, 'html.parser')
price = menu.select('.price')
print("Found on website : ")
print(price)

cost = 1999
available = False

for food in price:
    total = food.text[1:].replace(",", "")
    total = int(total)
    if total == cost:
        print("Price is still 1999")
    elif total > cost:
        print("Price is above 1999")
        print(total)
    else:
        print("Price is lower!!!")
        available = True
        print(total)

if available:
    message = client.messages.create(
        body='Grad Visa price reduced to ' + str(total) + '!',
        from_='****************',
        to='*************
    )
    print(message.sid)
else:
    print('Grad visa is the same price or more expensive.')






