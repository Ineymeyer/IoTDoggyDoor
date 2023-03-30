#!/usr/bin/env python3

import RPi.GPIO as GPIO
import os
from http.server import BaseHTTPRequestHandler, HTTPServer

host_name = '172.23.31.250'  # IP Address of Raspberry Pi
host_port = 8000
PIN = 14 #output to arduino for lock/unlock
btO = 8 #output to arduino for bluetooth track
lock_status = 'unknown'
pet_status = 'unknown'
global loginBool
loginBool = False
global loginAttempt
loginAttempt = False
global bluetoothTrack
bluetoothTrack = False
userName = 'test'
password = 'testtest'
inputUsr = 'unknown'
inputUsr = 'unknown'


def setupGPIO():
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    GPIO.setup(4, GPIO.IN, pull_up_down=GPIO.PUD_DOWN) #input from arduino to tell pi if door is locked or unlocked
    GPIO.setup(7, GPIO.IN, pull_up_down=GPIO.PUD_DOWN) #Input from esp32
    GPIO.setup(btO, GPIO.OUT) #output to arudion for bluetooth track
    GPIO.setup(PIN, GPIO.OUT) #output to arduion to lock or unlock
    GPIO.output(btO, GPIO.LOW)
    GPIO.output(PIN, GPIO.HIGH)

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
        '''
        if loginBool:

            html = '''
                <html>
                <head> 
                    <meta name="viewport" content="with=device-width, initial-scale=1.0">
                    <link rel="stylesheet" href="http://www.w3schools.com/lib/w3.css">
                    <meta http-equiv="refresh" content="3" >
                </head>
                    <body>
                    <section style="text-align:center">
                        <div style="padding-top:30px; background-color:slateblue; padding:30px 0px">
                            <h1><b>Welcome To Your Doggy Door!</b></h1>
                        </div>
                    </section>
                    <form action="/" method="POST">
                        <section style="text-align:center">
                            <div style="margin:1rem; padding:2rem 2rem; text-align:center">
                                <div style="display:inline-block; padding:1rem 1rem; vertical-align:middle">
                                    <h3><b>Basic Door Control:</b></h3>
                                    <div style="display:table; width:100%; height:auto; margin:10px 0px">
                                        <input type="submit" name="submit" value="Unlock" style="border-radius: 8px">
                                    </div>
                                    <div style="display:table; width:100%; height:auto; margin:10px 0px">
                                        <input type="submit" name="submit" value="Lock" style="border-radius: 8px">
                                    </div>
                                </div>
                                <div style="display:inline-block; padding:1rem 1rem; vertical-align:middle">
                                    <h3><b>Special Features:</b></h3>
                                    <div style="display:table; width:100%; height:auto; margin:10px 0px;">
                                        <input type="submit" name="submit" value="Bluetooth Track" style="border-radius: 8px">
                                        <label>   {}</label>
                                    </div>
                                    <h10>Door will unlock when your pet is close; otherwise, door will lock</h10>
                                    <div style="display:table; width:100%; height:auto; margin:10px 0px">
                                        <input type="submit" name="submit" value="Summon Pet" style="border-radius: 8px">
                                    </div>
                                    <h10>Play a sound to grab the attention of your pet</h10>
                                    <div style="display:table; width:100%; height:auto; margin:10px 0px">
                                        <input type="submit" name="submit" value="Release Treat" style="border-radius: 8px">
                                    </div>
                                    <h10>Dispense a treat for your pet</h10>
                                </div>
                            </div>
                        </section>
                    </form>
                    <section style="text-align: center">
                        <h5>Lock status: {}</h5>
                        <h5>Pet status: {}</h5>
                    </section>
                    </body>
                </html>
            '''
        elif loginAttempt:
            html = '''
                <html>
                <head> 
                    <meta name="viewport" content="with=device-width, initial-scale=1.0">
                    <link rel="stylesheet" href="http://www.w3schools.com/lib/w3.css">
                </head>
                    <body>
                    <section style="text-align:center">
                        <div style="padding-top:30px">
                            <h1>Please Login to Your Doggy Door!</h1>
                        </div>
                    </section>
                    <form action="/" method="POST">
                        <section style="text-align:center">
                            <div style="margin:1rem; padding:2rem 2rem; text-align:center">
                                <div style="display:inline-block; padding:1rem 1rem; vertical-align:middle">
                                    <h3>Login:</h3>
                                    <div style="display:table; width:100%; height:auto; margin:10px 0px">
                                        <label for="usr">Username:</label>
                                        <input id="usr" name="usr">
                                        <label for="pwd">Password:</label>
                                        <input type="password" id="pwd" name="pwd">
                                        <div style="display:table; width:100%; height:auto; margin:10px 0px">
                                            <input type="submit" name="submit" value="Login">
                                        </div>
                                        <h3>Invalid Credentials</h3>
                                    </div>
                                </div>
                            </div>
                        </section>
                    </form>
                    </body>
                </html>
            '''
        else:

            html = '''
                <html>
                <head> 
                    <meta name="viewport" content="with=device-width, initial-scale=1.0">
                    <link rel="stylesheet" href="http://www.w3schools.com/lib/w3.css">
                </head>
                    <body>
                    <section style="text-align:center">
                        <div style="padding-top:30px">
                            <h1>Please Login to Your Doggy Door!</h1>
                        </div>
                    </section>
                    <form action="/" method="POST">
                        <section style="text-align:center">
                            <div style="margin:1rem; padding:2rem 2rem; text-align:center">
                                <div style="display:inline-block; padding:1rem 1rem; vertical-align:middle">
                                    <h3>Login:</h3>
                                    <div style="display:table; width:100%; height:auto; margin:10px 0px">
                                        <label for="usr">Username:</label>
                                        <input id="usr" name="usr">
                                        <label for="pwd">Password:</label>
                                        <input type="password" id="pwd" name="pwd">
                                        <div style="display:table; width:100%; height:auto; margin:10px 0px">
                                            <input type="submit" name="submit" value="Login">
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </section>
                    </form>
                    </body>
                </html>
            '''


        self.do_HEAD()
        state = GPIO.input(4) #reading in from arduino if door is locked or unlocked
        petNearby = GPIO.input(7) # reading in from esp to see if tag is nearby

        if (state):
            lock_status = 'Locked'
        else:
            lock_status = 'Unlocked'
        if (petNearby):
            pet_status = 'Nearby'
        else:
            pet_status = 'NOT Nearby'
        global bluetoothTrack
        if bluetoothTrack:
            currentBTTrack = 'ON'
        else:
            currentBTTrack = 'OFF'
        print("HERE")
        print(bluetoothTrack)
        self.wfile.write(html.format(currentBTTrack, lock_status, pet_status).encode("utf-8"))

    def do_POST(self):

        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length).decode("utf-8")
        global loginBool
        if loginBool:
            post_data = post_data.split("=")[1]
        else:
            post_data = post_data[:-13] #getting input from user for username and password
            post_data = post_data[4:]
            tempUsr, tempPass = post_data.split('&pwd=')
            
            if tempUsr == 'test' and tempPass == 'test123':
                loginBool = True
            else:
                global loginAttempt
                loginAttempt = True
        print("NOW")
        print(post_data)
        if post_data == 'Login':
            loginBool = True
            
        elif post_data == 'Bluetooth+Track':
            global bluetoothTrack
            if bluetoothTrack:
                bluetoothTrack = False
                GPIO.output(btO, GPIO.LOW) #telling arduino to not listen to esp for tracking
            else:
                bluetoothTrack = True
                GPIO.output(btO, GPIO.HIGH) #telling arduino to listen to esp for tracking


        elif post_data == 'Lock':
            GPIO.output(PIN, GPIO.HIGH)
            lock_status = 'locked'
            print(lock_status)
        else:
            GPIO.output(PIN, GPIO.LOW)
            lock_status = 'unlocked'
            print(lock_status)

        print("LED is {}".format(post_data))
        print("login is : {}".format(loginBool))
        print("login attempt is : {}".format(loginAttempt))
        self._redirect('/')  # Redirect back to the root url


# # # # # Main # # # # #

if __name__ == '__main__':
    http_server = HTTPServer((host_name, host_port), MyServer)
    print("Server Starts - %s:%s" % (host_name, host_port))

    try:
        http_server.serve_forever()
    except KeyboardInterrupt:
        http_server.server_close()
