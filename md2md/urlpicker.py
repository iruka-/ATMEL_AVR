#!/usr/bin/env python
# -*- coding: utf-8 -*-

#
# ../md/*.md ファイルを総なめ.
#    psp.dip.jpからのリンクを全部列挙する.
#    リンクの置き換え.
#

import os
import sys
import glob

MYURL='http://psp.dip.jp/'

TOURL='https://github.com/iruka-/ATMEL_AVR/blob/master/'

f_replace=1

# URlハッシュ登録.
shash={}

def loadfile(fname):
    """
    ファイルの読み込み ==> buf を返す.
    """
    f = open(fname,'r');
    buf = f.read();
    f.close();
    return buf;

def is_urlchr(c):
    if(c.isalnum()):
        return True
    if(c=='/'):
        return True
    if(c=='.'):
        return True
    if(c=='-'):
        return True
    if(c=='_'):
        return True

    return False



def treat_url1(line):
    """
    １ URL を処理.
    """
#    print ">"+line+"<"
    global shash
    line = line.replace('upload.cgi/','upload/')
    shash[line]=line


def treat_url(line):
    """
    １ URL を切り取り.
    """
    j=len(MYURL)
    m=len(line)
    while(j<m):
        c=line[j]
        if(is_urlchr(c)==False):
            treat_url1(line[0:j])
            return
        j+=1

def treat_line(line,idx):
    """
    １行を処理.
    """
    line = line[idx:]
    treat_url(line)

    # 末尾再帰
    idx = line.find(MYURL)
    if(idx>0):
        treat_line(line,idx)

def treat_file(file):
    """
    １ファイルを処理.
    """
    buf = loadfile(file)
    bufs=buf.split("\n")
    for line in bufs:
        idx = line.find(MYURL)
        if(idx>=0):
            treat_line(line,idx)


def globdir(dir):
    """
    ディレクトリ内のファイル舐め.
    """
#    subdirs = [os.path.basename(x) for x in glob.glob('module/*')]
    subdirs = glob.glob(dir)
    for subdir in subdirs:
        treat_file(subdir)

def replace_file(file):
    """
    １ファイルを処理.
    """
    buf = loadfile(file)
    bufs=buf.split("\n")

    f = open(file,'w');
    for line in bufs:
        s = line
        s=s.replace(MYURL+'web/upload.cgi/',MYURL+'web/upload/',9)
        s=s.replace(MYURL,TOURL,9)
        f.write(s)
        f.write("\n")
    
    f.close();
    


def replace_md(dir):
    """
    ディレクトリ内のファイル書き換え.
    """
    subdirs = glob.glob(dir)
    for subdir in subdirs:
        replace_file(subdir)

def print_url(url):
    url=url.replace(MYURL,'')
    print url

def printhash():
    global shash
    buf=[]
    for url in shash:
        buf.append(url)

    buf = sorted(buf)
    for url in buf:
#        print url
        print_url(url)

def main():
    if(f_replace):
        replace_md('../md/*.md')
        return

    globdir('../md/*.md')
    printhash()

#
if __name__ == "__main__":
    main()

