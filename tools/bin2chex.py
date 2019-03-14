import sys
import getopt

filename = None

opts,args = getopt.getopt(sys.argv[1:],'f:')
for o,a in opts:
  if o == '-f':
    filename = a


with open(filename, "rb") as f:
  while True:
    block = f.read(16)
    if not block:
      break
    print ''.join('0x{:02X}, '.format(ord(c)) for c in block)
