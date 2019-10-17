import os
import termios
import tkinter as tk
import threading
import struct
import time

termatt = [0, 4, 3261, 35384, termios.B9600, termios.B9600, [b'\x03', b'\x1c', b'\x7f', b'\x15', b'\x04', 0, 1, b'\x00', b'\x11', b'\x13', b'\x1a', b'\x00', b'\x12', b'\x0f', b'\x17', b'\x16', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00']]

class MainApp:
    def __init__(self):
        self.tk = tk.Tk()
        self.tk.title("TTY-Hexdump")
        self.tty = None
        self.commthread = None
    def run(self):
        self.tty = os.open('/dev/ttyUSB1', os.O_RDONLY)
        attold = termios.tcgetattr(self.tty)
        termios.tcsetattr(self.tty, termios.TCSANOW, termatt)
        self.mainwnd = MainWnd(self.tk, self)
        self.mainwnd.pack(fill=tk.BOTH, expand=1)
        self.tk.protocol("WM_DELETE_WINDOW", self.mainwnd.quit)
        self.commthread = CommThread(self, self.tty)
        self.commthread.start()
        self.mainwnd.mainloop()
        self.commthread.exit = True
        self.tk.destroy()
        self.commthread.join(5.0)
        termios.tcsetattr(self.tty, termios.TCSANOW, attold)
        os.close(self.tty)

class MainWnd(tk.Frame):
    def __init__(self, parent, app):
        tk.Frame.__init__(self, parent)
        self.app = app

        #self.grid_rowconfigure(0, weight=1)
        #self.grid_columnconfigure(0, weight=1)
        
        self.textedit = tk.Text(self)
        self.textedit.pack()
        self.textedit.insert(tk.END, 'Hello world\n')

class CommThread(threading.Thread):
    def __init__(self, owner, tty):
        threading.Thread.__init__(self)
        self.owner = owner
        self.tty   = tty
        self.exit  = False
        self.timer = None
        self.time  = None
    def run(self):
        self.exit = False
        while(not self.exit):
            bindata = bytearray(b'')
            while len(bindata) < 10 :
                c = os.read(self.tty, 1)
                bindata.extend(c)
                if len(bindata) == 1:
                    if bindata[0] != 0x12 :
                        bindata = bytearray(b'')
                    else :
                        self.time = time.perf_counter()
                        # print('Starting time: {0:.5f}'.format(self.time))
                if len(bindata) > 1:
                    delta = time.perf_counter() - self.time
                    # print('delta: {0:.5f}'.format(delta))
                    if delta > 0.1:
                        bindata = bytearray(b'')
                        bindata.extend(c)
                        self.time = time.perf_counter()
            if len(bindata) == 10 :
                data = struct.unpack('BBBBBBBBBB', bindata)
                string = '%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x' % data
                if not self.exit :
                    print('Data received: ' + string)
                    self.owner.mainwnd.textedit.insert(tk.END, string)
                    self.owner.mainwnd.textedit.insert(tk.END, '\n')
                    self.owner.mainwnd.textedit.yview_moveto(1.0)
            else:
                print('Invalid length: {0:d}'.format(len(bindata)))

    def timerfunc(self):
        self.timer_expired = True
                    
                
                
                
            
        
app = MainApp()
app.run()


