#!/usr/bin/python

from selenium import webdriver
import sys
from selenium.webdriver.common.keys import Keys

if len(sys.argv) <= 2:
	print "You need to supply username and password"
	exit(1)

driver = webdriver.Firefox()
driver.get('http://vutbr.cz/login')
u = driver.find_element_by_css_selector('input[name="LDAPlogin"]')
u.send_keys(sys.argv[1])
p = driver.find_element_by_css_selector('input[name="LDAPpasswd"]')
p.send_keys(sys.argv[2])
p.send_keys(Keys.RETURN)
# what you want with driver
# driver.close()
