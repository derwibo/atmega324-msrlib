import os
import termios
import tkinter as tk
import threading
import struct

#termatt = [0, 4, 3261, 35384, termios.B38400, termios.B38400, [b'\x03', b'\x1c', b'\x7f', b'\x15', b'\x04', 0, 1, b'\x00', b'\x11', b'\x13', b'\x1a', b'\x00', b'\x12', b'\x0f', b'\x17', b'\x16', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00']]
termatt = [0, 4, 3261, 35384, termios.B19200, termios.B19200, [b'\x03', b'\x1c', b'\x7f', b'\x15', b'\x04', 0, 1, b'\x00', b'\x11', b'\x13', b'\x1a', b'\x00', b'\x12', b'\x0f', b'\x17', b'\x16', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00']]
termatt = [0, 4, 3261, 35384, termios.B9600, termios.B9600, [b'\x03', b'\x1c', b'\x7f', b'\x15', b'\x04', 0, 1, b'\x00', b'\x11', b'\x13', b'\x1a', b'\x00', b'\x12', b'\x0f', b'\x17', b'\x16', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00', b'\x00']]


ttydev = '/dev/ttyUSB1'

class MainApp:
    def __init__(self):
        self.tk = tk.Tk()
        self.tk.title("Py-PPS")
        self.tty = None
        self.commthread = None
    def run(self):
        self.tty = os.open(ttydev, os.O_RDONLY)
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
        
        self.v = tk.StringVar()
        self.label = tk.Label(self, textvariable=self.v).pack()
        self.v.set('00 00 00 00 00 00 00 00 00 00')

class CommThread(threading.Thread):
    def __init__(self, owner, tty):
        threading.Thread.__init__(self)
        self.owner = owner
        self.tty   = tty
        self.exit  = False
    def run(self):
        self.exit = False
        cnt = 0
        while(not self.exit):
            c = os.read(self.tty, 1)
            if len(c) > 0 and c[0] == 0x12 :
                n = os.read(self.tty, 1)
                bindata = bytearray(b'')
                while len(bindata) < 8 :
                    bindata.extend(os.read(self.tty, 1))
                if len(bindata) == 8 :
                    # print('Data received')
                    data = struct.unpack('HHHH', bindata)
                    cnt = cnt + 1
                    string = '%d - ' % cnt + '%d %d %d %d' % data
                    if not self.exit :
                        self.owner.mainwnd.v.set(string)
                else:
                    print('Invalid length: {0:d}'.format(len(bindata)))
                    
                
                
                
            
        
app = MainApp()
app.run()


