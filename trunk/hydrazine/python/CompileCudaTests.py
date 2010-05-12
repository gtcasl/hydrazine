#! /usr/bin/env python

################################################################################
#	\file CompileCuda.py
#	\author Gregory Diamos <gregory.diamos@gatech.edu>
#	\date Wednesday May 12, 2010
#	\brief A script that performs CUDA to C++ conversion
##

import os
from optparse import OptionParser

class CudaSource:
	def __init__(self, path):
		self.filename = os.path.abspath(path)
		self.outfile = self.filename + ".cpp"
		
def getAllCudaSources(path):
	sources = []
	for dirpath, dirnames, filenames in os.walk(path):
		for filename in filenames:
			name = os.path.join(dirpath, filename)
			if os.path.isfile(name):
				split = name.rsplit('.', 1)
				extension = ""
				if len(split) == 2:
					extension = split[1]
				if extension == "cu":
					sources.append(CudaSource(name))
	return sources

def compileSources(commandBase, sources):
	for source in sources:
		if !os.path.isfile(source.outfile):
			command = commandBase + " -o " + source.outfile 
				+ " " + source.filename
			print command
			os.system(command)

def sanitizeSources(sources):
	for source in sources:
		if os.path.isfile(source.outfile):
			print "sanitizing " + os.path.basename(source.outfile)
			inFile = open(source.outfile, 'r')
			text = inFile.read()
			inFile.close()
			text = text.replace('stdarg', 'va')
			text = text.replace('auto', '')
			outFile = open(source.outfile, 'w')
			outFile.write(text)

def clean(sources):
	for source in sources:
		if os.path.isfile(source.outfile):
			print "deleting file " + source.outfile
			os.remove(source.outfile)
		
def main():
	parser = OptionParser()
	
	parser.add_option("-d", "--directory", action="store", 
		default=".", dest="directory", help="The directory to run on.")
	parser.add_option("-a", "--arguments", action="store", 
		default="-I ~/checkout/thrust -I ./sdk", dest="arguments", 
		help="NVCC options.")
	parser.add_option("-c", "--clean", action="store_true", 
		dest="clean", default=False, help="Delete all .cu.cpp files.")
	parser.add_option("-s", "--sanitize", action="store_true", 
		dest="sanitize", default=False, help="Only sanitize .cu.cpp files.")
	
	(options, args) = parser.parse_args()
	
	command = "nvcc --cuda " + options.arguments
	path = os.getcwd()
	sources = getAllCudaSources(options.directory)
	if options.clean:
		clean(sources)
	elif options.sanitize:
		sanitizeSources(sources)
	else:
		compileSources(command, sources)
		sanitizeSources(sources)
	
if __name__ == "__main__":
    main()
	
