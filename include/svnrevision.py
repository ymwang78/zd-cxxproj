# -*-coding:gbk-*-
import os, re, tempfile, time

tmp = tempfile.mktemp()
svn_cmd = "svn update -N . > " + tmp
os.system(svn_cmd)
file = open(tmp,'r')
dst_line = file.readline()
file.close()
p = re.compile('\d+')
m = p.search(dst_line)

file = open("include/svnrevision.h", 'w')
file.write('#ifndef __svnrevision_h__\r\n')
file.write('#define __svnrevision_h__\r\n')
file.write('#define SVNREVISION \"')
file.write(m.group())
file.write('\"\r\n#endif /*__svnrevision_h__*/\r\n')
#try:
#    print "The revision:",m.group()
#except:
#    print "Get the revision failed."
os.unlink(tmp)
#time.sleep(2)

