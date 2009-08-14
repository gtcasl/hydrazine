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
	def __init__( self, path, barWidth, color, verbose ) :
		self.path = path
		self.verbose = verbose
		self.barWidth = barWidth
		self.xlabel=""
		self.ylabel=""
		self.title=""
		self.barWidth=.35
		self.setNames=[]
		self.arguments=[]
		self.colors=[]
		self.defaultColor=color
		self.log = False
		self.position = 'best'
		
	def parse( self ):
		self.names = {}				
		inputs = open( self.path, 'r' )
		self.parseNames(inputs)
		self.parseArguments( inputs )
		self.parseData( inputs )
		self.partition()
		assert self.size > 0
		while len( self.arguments ) < self.size:
			self.arguments.append("")
		while len( self.colors ) < self.size:
			self.colors.append(self.defaultColor)
		assert len( self.arguments ) == self.size
		self.barWidth=min(self.barWidth, 1/(self.size + 1.0))
		
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
			elif temp.startswith("position ") :
				self.position = temp[9:].strip('\n')
			elif temp.startswith("title ") :
				self.title = temp[6:]
			elif temp.startswith("barwidth ") :
				self.barWidth = float(temp[9:])
			elif temp.startswith("labels ") :
				for name in temp[7:].split():
					self.setNames.append(name)
			elif temp.startswith("colors ") :
				for name in temp[7:].split():
					self.colors.append(name)
			elif temp.startswith("log ") :
				if temp.find( "True" ):
					self.log = True
				else:
					self.log = False
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
		self.names = { }
		self.size = -1
		for name in inputs:
			if name == "\n" :
				continue
			items = name.split()
			if items[ 0 ] in self.names:
				raise Exception, "Duplicate type " + items[ 0 ] + " declared"
			self.names[ items[ 0 ] ] = [ ]
			if self.size == -1 :
				self.size = len( items ) - 1
			if self.size != len( items ) - 1:
				raise Exception, "Label " + items[ 0 ] + " only has " \
					+ str( len( items ) - 1 ) + " elements"
			for i in range( 1, len( items ) ):
				self.names[ items[ 0 ] ].append( float( items[ i ] ) )
	
	def partition( self ):
		self.indicies = range( len( self.names ) )
		self.labels = [ ]
		self.dataVector = [ ]
		count = 0
		for name, data in self.names.iteritems() :
			self.labels.append( name )
			if count == 0:
				for i in data:
					self.dataVector.append( [ ] )
			index = 0
			for i in data:
				self.dataVector[ index ].append( i )
				index += 1
			count += 1
		
	def display( self ):
		self.parse()
		plot.figure( 1 )
		plots = [ ]
		count = 0
		for data in self.dataVector:
			plots.append( plot.bar( numpy.array( self.indicies ) 
				+ count * self.barWidth, numpy.array( data ), 
				self.barWidth, color = self.colors[ count ], 
				log = self.log )[0] )
			count += 1
		plot.xlabel( self.xlabel )
		plot.ylabel( self.ylabel )
		plot.title( self.title )
		plot.xticks( numpy.array(self.indicies) + numpy.array(self.barWidth)
			* ( self.size / 2.0 ), self.labels, rotation = 'vertical' )
		print "Plots " + str(len(plots)) + ", names " + str(len(self.setNames))
		plot.legend( plots, self.setNames, self.position )
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
	parser.add_option( "-c", "--default_color", default="k" )
	
	( options, arguments ) = parser.parse_args()
	
	plot = Plot( options.inputFile, options.barWidth, options.default_color, 
		options.verbose )
	plot.display()

################################################################################

################################################################################
## Guard Main
if __name__ == "__main__":
	main()

################################################################################

