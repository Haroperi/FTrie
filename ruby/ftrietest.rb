require 'ftrie'
require 'pp'

def byte_seq str
	a = []
	str.split(//).each { |c|
		a.push 0
		c.unpack("C*").each { |byte|
			a[-1] = a[-1]*256+byte
		}
	}
	return a
end

ftrie = Ftrie.new

while l = gets
	l.chomp!
	seq = byte_seq l
	ftrie.insert seq
end

while l = gets
	l.chomp!
	seq = byte_seq l
	cost = (l.size*0.35).to_i
	pp ftrie.fuzzysearch(seq, cost)
	puts cost
end

