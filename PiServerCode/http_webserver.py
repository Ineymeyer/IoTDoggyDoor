#!/usr/bin/env python3

import RPi.GPIO as GPIO
import os
from http.server import BaseHTTPRequestHandler, HTTPServer

host_name = '172.23.31.250'  # IP Address of Raspberry Pi
host_port = 8000
PIN = 14
lock_status = 'unknown'

def setupGPIO():
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    GPIO.setup(4, GPIO.IN)
    GPIO.setup(PIN, GPIO.OUT)


class MyServer(BaseHTTPRequestHandler):

    setupGPIO()

    def do_HEAD(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()

    def _redirect(self, path):
        self.send_response(303)
        self.send_header('Content-type', 'text/html')
        self.send_header('Location', path)
        self.end_headers()

    def do_GET(self):
        html = '''
            <html>
            <head> 
                <meta name="viewport" content="with=device-width, initial-scale=1.0">
                <link rel="stylesheet" href="http://www.w3schools.com/lib/w3.css">
                <meta http-equiv="refresh" content="3" >
            </head>
                <body>
                <section style="text-align:center">
                    <div style="padding-top:30px">
                        <h1>Welcome To Your Doggy Door!</h1>
                    </div>
                </section>
                <form action="/" method="POST">
                    <section style="text-align:center">
                        <div style="margin:1rem; padding:2rem 2rem; text-align:center">
                            <div style="display:inline-block; padding:1rem 1rem; vertical-align:middle">
                                <h3>Basic Door Control:</h3>
                                <div style="display:table; width:100%; height:auto; margin:10px 0px">
                                    <input type="submit" name="submit" value="Unlock">
                                </div>
                                <div style="display:table; width:100%; height:auto; margin:10px 0px">
                                    <input type="submit" name="submit" value="Lock">
                                </div>
                            </div>
                            <div style="display:inline-block; padding:1rem 1rem; vertical-align:middle">
                                <h3>Special Features:</h3>
                                <div style="display:table; width:100%; height:auto; margin:10px 0px;">
                                    <input type="submit" name="submit" value="Bluetooth Track">
                                </div>
                                <h10>Door will unlock when your pet is close; otherwise, door will lock</h10>
                                <div style="display:table; width:100%; height:auto; margin:10px 0px">
                                    <input type="submit" name="submit" value="Summon Pet">
                                </div>
                                <h10>Play a sound to grab the attention of your pet</h10>
                                <div style="display:table; width:100%; height:auto; margin:10px 0px">
                                    <input type="submit" name="submit" value="Release Treat">
                                </div>
                                <h10>Dispense a treat for your pet</h10>
                            </div>
                        </div>
                    </section>
                </form>
                <section>
                    <h10>Lock status: {}</h10>
                </section>
                </body>
            </html>
        '''

        self.do_HEAD()
        state = GPIO.input(4)
        if (state):
            lock_status = 'Locked'
        else:
            lock_status = 'Unlocked'
        
        self.wfile.write(html.format(lock_status).encode("utf-8"))

    def do_POST(self):

        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length).decode("utf-8")
        post_data = post_data.split("=")[1]

        

        if post_data == 'Lock':
            GPIO.output(PIN, GPIO.HIGH)
            lock_status = 'locked'
            print(lock_status)
        else:
            GPIO.output(PIN, GPIO.LOW)
            lock_status = 'unlocked'
            print(lock_status)

        print("LED is {}".format(post_data))
        self._redirect('/')  # Redirect back to the root url


# # # # # Main # # # # #

if __name__ == '__main__':
    http_server = HTTPServer((host_name, host_port), MyServer)
    print("Server Starts - %s:%s" % (host_name, host_port))

    try:
        http_server.serve_forever()
    except KeyboardInterrupt:
        http_server.server_close()
