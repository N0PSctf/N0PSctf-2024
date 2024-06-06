from flask import Flask, request, render_template_string, render_template
import re

app = Flask(__name__)

app.valid_code = 'SSTI-1337-Templ4Te-1nJ3cT10N'

page = '''{%% extends "base.html" %%}
{%% block title %%}%s{%% endblock %%}
{%% block content %%}
<p class='message'>%s</p>
{%% endblock %%}'''

@app.route('/', methods=['GET', 'POST'])
def index():

    if request.method == 'POST':

        regex = "[A-Z]{4}-[0-9]{4}-.{4,}"
        code = request.form['code']
        code = re.sub("[ ]+", '', re.sub("[\}]+", '}', re.sub("[\{]+", '{', code)))
        if not re.match(regex, code):
            return render_template_string(page % ('Invalid code', 'Invalid code format'))
        if code == app.valid_code:
            return render_template_string(page % ('Valid code!', 'This code is valid! You have won a gift!'))
        else:
            return render_template_string(page % ('Invalid code', 'The code <pre>%s</pre> is invalid.' % (code)))
    else:
        return render_template('base.html')

@app.errorhandler(404)
def pageNotFound(error):
    return render_template_string(page % ('Page not found', 'This page does not exist')), 404

@app.errorhandler(500)
def serverError(error):
    return render_template_string(page % ('Server error', 'There was an error on the server')), 500


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80)
