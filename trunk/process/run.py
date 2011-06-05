#/usr/bin/env python
import pexpect, sys, os, time, dircache

TIMEOUT=20
serverfilename = 'u3khan12345.s19'

# compile our code
if os.system('make') != 0:
		print 'Error compiling code'
else:
		print 'Compilation successful!'

# find .s19 file
list = dircache.listdir(".")
s19file = None
for file in list:
	extension = os.path.splitext(file)[1]
	if extension == '.s19':
		s19file = file

# exit if .s19 file was not found
if s19file is None:
	print 'Unable to find s19 file. Exiting...'
	sys.exit(0)

# copy our stuff to cfserver
if os.system('cfcp %s %s > /dev/null' % (s19file, serverfilename)) != 0:
        print 'Error copying firmware'
else:   
        print 'Successfully copied %s to cf-server as %s' %(s19file, serverfilename)

cfserver = pexpect.spawn('telnet cf-server')

# grab the 2nd serial port info
cfserver.expect('Second Serial Port On Port: ([0-9]*\n)')
PORT = int(cfserver.match.group(1))

# select "continue executing" in cf-server
cfserver.expect('Please Pick', timeout=TIMEOUT)
cfserver.expect(': ', timeout=TIMEOUT)
cfserver.send('1')
print "Started JanusROM"
time.sleep(1)

# wait for the janusROM prompt to init tftp
cfserver.expect('janusROM> ', timeout=TIMEOUT)
cfserver.send('tftp -f %s\r\n' % serverfilename)
cfserver.expect('Finished. Start Address is [0-9a-fA-Fx]+\.', timeout=TIMEOUT)
print "Successfully loaded %s" % serverfilename

# make your life easier
print "Second serial port on port: %i" % PORT
print "telnet cf-server %i" % PORT
cfserver.interact()

# clean up
if os.system('make clean') != 0:
		print 'Error cleaning up'
else:
		print 'Clean up successful!'
