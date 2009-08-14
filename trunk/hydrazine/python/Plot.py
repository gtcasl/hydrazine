################################################################################
##	\file RunRegression.py
##	\author Gregory Diamos
##	\date July 19, 2008
##	\brief A class and script for parsing a list of data elements and 
##	identifiers, and plotting them using Matplotlib
################################################################################

from optparse import OptionParser

import matplotlib.pyplot as plot
import matplotlib.text as text
import matplotlib.font_manager as font_manager
import numpy

################################################################################
## Plot - A class that parses an input file and creates a plot
class Plot:
	def __init__( self, path, barWidth, verbose ) :
		self.path = path
		self.verbose = verbose
		self.barWidth = barWidth
		self.xlabel=""
		self.ylabel=""
		self.title=""
		self.barWidth=.35
		self.setNames=[]
		self.arguments=[]
		
	def parse( self ):
		self.names = {}				
		inputs = open( self.path, 'r' )
		self.parseNames(inputs)
		self.parseArguments( inputs )
		self.parseData( inputs )
		self.partition()
		assert self.size > 0
		while len(self.arguments) < self.size:
			self.arguments.append("")
		assert len(self.arguments) == self.size
		
	def parseNames( self, inputs ):
		while True:
			temp = inputs.readline();
			if len( temp) == 0:
				break
			elif temp == '\n':
				continue
			elif temp.startswith("xlabel ") :
				self.xlabel = temp[7:]
			elif temp.startswith("ylabel ") :
				self.ylabel = temp[7:]
			elif temp.startswith("title ") :
				self.title = temp[6:]
			elif temp.startswith("barwidth ") :
				self.barWidth = float(temp[10:])
			elif temp.startswith("labels ") :
				for name in temp[7:].split():
					self.setNames.append(name)
			elif temp.find( "--arguments--" ) != -1 :
				break
			
				
	def parseArguments( self, inputs ):
		while True:
			temp = inputs.readline();
			if len( temp) == 0:
				break
			elif temp == '\n':
				continue
			elif temp.find( "--data--" ) != -1 :
				break
			else:
				self.arguments.append(temp)
			
	def parseData( self, inputs ):
		self.names = {}
		self.size = -1
		for name in inputs:
			if name == "\n" :
				continue
			items = name.split()
			if items[0] in self.names:
				raise Exception, "Duplicate type " + items[0] + " declared"
			self.names[ items[0] ] = []
			if self.size == -1 :
				self.size = len( items ) - 1
			if self.size != len( items ) - 1:
				raise Exception, "Label " + items[0] + " only has " \
					+ str(len( items ) - 1) + " elements"
			for i in range( 1, len( items ) ):
				self.names[ items[0] ].append( float(items[i]) )
	
	def partition( self ):
		self.indicies = numpy.arange( len( self.names ) )
		labels = []
		dataVector = []
		count = 0
		for name, data in self.names.iteritems() :
			labels.append( name )
			if count == 0:
				for i in data:				
					dataVector.append([])
			index = 0
			for i in data:
				dataVector[index].append(i)
				++index
			++count
		self.labels = numpy.array(labels)
		self.dataVector = []
		for i in dataVector:
			self.dataVector.append( numpy.array(i) )
		
		
	def display( self ):
		self.parse()
		plot.figure(1)
		plots = []
		count = 0
		for data in self.dataVector:
			plots.append( plot.bar( self.indicies, data, self.barWidth ) )
			++count
		plot.xlabel( self.xlabel )
		plot.ylabel( self.ylabel )
		plot.title( self.title )
		plot.xticks( self.indicies + self.barWidth * ( count / 2.0 ), 
			self.labels, rotation='vertical' )
		plot.show()

################################################################################


################################################################################
## Main
def main():
	parser = OptionParser()
	
	parser.add_option( "-i", "--inputFile", \
		default="plot.in" )
	parser.add_option( "-v", "--verbose", default = False, \
		action = "store_true" )
	parser.add_option( "-b", "--barWidth", default = .35 )
	
	( options, arguments ) = parser.parse_args()
	
	plot = Plot( options.inputFile, options.barWidth, options.verbose )
	plot.display()

################################################################################

################################################################################
## Guard Main
if __name__ == "__main__":
	main()

################################################################################

