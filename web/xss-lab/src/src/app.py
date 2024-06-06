from flask import Flask, render_template, request
import urllib.parse
from selenium import webdriver 
from selenium.webdriver.chrome.options import Options

app = Flask(__name__)

def filter_2(payload):
    return payload.lower().replace("script", "").replace("img", "").replace("svg", "")

def filter_3(payload):
    if "://" in payload.lower():
        return "Nope"
    if "document" in payload.lower():
        return "Nope"
    if "cookie" in payload.lower():
        return "Nope"
    return payload.lower().replace("script", "").replace("img", "").replace("svg", "")

def filter_4(payload):
    if any(c in payload for c in '+"/'):
        return "Nope"
    if "://" in payload.lower():
        return "Nope"
    if "document" in payload.lower():
        return "Nope"
    if "cookie" in payload.lower():
        return "Nope"
    return payload.replace("script", "").replace("img", "").replace("svg", "")

@app.route('/', methods=['GET', 'POST'])
def xss1():
    if request.method == 'GET':
        payload = request.args.get('payload')
        if not payload:
            payload = ""
        return render_template("xss1.html", payload=payload)
    elif request.method == 'POST':
        try:
            chrome_options = Options()
            chrome_options.add_argument("--no-sandbox") # linux only
            chrome_options.add_argument("--headless=new") # for Chrome >= 109
            driver = webdriver.Chrome(options=chrome_options)
            driver.get("http://localhost/")
            url = "http://localhost/?payload=" + urllib.parse.quote_plus(request.form["payload"])
            driver.add_cookie({"name": "xss2", "value": "/bf2a73106a3aa48bab9b8b47e4bd350e", "domain": "localhost"})
            driver.get(url)
            #print(driver.page_source.encode("utf-8"))
            driver.quit()
            return 'Page visited!'
        except:
            return 'An error occured.'


@app.route("/bf2a73106a3aa48bab9b8b47e4bd350e", methods=['GET', 'POST'])
def xss2():
    if request.method == 'GET':
        payload = request.args.get('payload')
        if not payload:
            payload = ""
        return render_template("xss2.html", payload=filter_2(payload))
    elif request.method == 'POST':
        try:
            chrome_options = Options()
            chrome_options.add_argument("--no-sandbox") # linux only
            chrome_options.add_argument("--headless=new") # for Chrome >= 109
            driver = webdriver.Chrome(options=chrome_options)
            driver.get("http://localhost/")
            url = "http://localhost/bf2a73106a3aa48bab9b8b47e4bd350e?payload=" + urllib.parse.quote_plus(request.form["payload"])
            driver.add_cookie({"name": "xss3", "value": "/3e79c8a64bd10f5fa897b7832384f043", "domain": "localhost"})
            driver.get(url)
            #print(driver.page_source.encode("utf-8"))
            driver.quit()
            return 'Page visited!'
        except:
            return 'An error occured.'
        
@app.route("/3e79c8a64bd10f5fa897b7832384f043", methods=['GET', 'POST'])
def xss3():
    if request.method == 'GET':
        payload = request.args.get('payload')
        if not payload:
            payload = ""
        return render_template("xss3.html", payload=filter_3(payload))
    elif request.method == 'POST':
        try:
            chrome_options = Options()
            chrome_options.add_argument("--no-sandbox") # linux only
            chrome_options.add_argument("--headless=new") # for Chrome >= 109
            driver = webdriver.Chrome(options=chrome_options)
            driver.get("http://localhost/")
            url = "http://localhost/3e79c8a64bd10f5fa897b7832384f043?payload=" + urllib.parse.quote_plus(request.form["payload"])
            driver.add_cookie({"name": "xss4", "value": "/f40e749b80cff27f8e726b2a95740dd6", "domain": "localhost"})
            driver.get(url)
            #print(driver.page_source.encode("utf-8"))
            driver.quit()
            return 'Page visited!'
        except:
            return 'An error occured.'
        
@app.route("/f40e749b80cff27f8e726b2a95740dd6", methods=['GET', 'POST'])
def xss4():
    if request.method == 'GET':
        payload = request.args.get('payload')
        if not payload:
            payload = ""
        return render_template("xss4.html", payload=filter_4(payload))
    elif request.method == 'POST':
        try:
            chrome_options = Options()
            chrome_options.add_argument("--no-sandbox") # linux only
            chrome_options.add_argument("--headless=new") # for Chrome >= 109
            driver = webdriver.Chrome(options=chrome_options)
            driver.get("http://localhost/")
            url = "http://localhost/f40e749b80cff27f8e726b2a95740dd6?payload=" + urllib.parse.quote_plus(request.form["payload"])
            driver.add_cookie({"name": "flag", "value": "N0PS{cR05s_S1t3_Pr0_5cR1pT1nG}", "domain": "localhost"})
            driver.get(url)
            #print(driver.page_source.encode("utf-8"))
            driver.quit()
            return 'Page visited!'
        except:
            return 'An error occured.'

if __name__ == '__main__':
   app.run(host='0.0.0.0', port=80)