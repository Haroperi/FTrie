#coding:utf-8
require 'benchmark'
require 'ftrie'
require_relative 'Trie.rb'

# pick one element at random
class Array
	def pick
		self[rand(self.size)]
	end
end

# generate random sequence
def random_seq
	str = []
	len = rand(10)+1
	len.times { str.push CHARACTERS.pick }
	return str.join
end

def byte_seq str
	a = []
	str.split(//).each do |c|
		a.push 0
		c.unpack("C*").each do |byte|
			a[-1] = a[-1]*256+byte
		end
	end
	return a
end

def cost str
	(0.35 * str.size).to_i
end

CHARACTERS = %w[a b c d e f g h i j k l m n o p q
	r s t u v w x y z A B C D E F G H I
	J K L M N O P Q R S T U V W X Y Z
]

if ARGV.size != 2
	STDERR.puts 'usage: <number of words> <seed>'
	exit
end

trie = Trie.new
ftrie = Ftrie.new
N = ARGV[0].to_i	# 辞書に登録する単語数
M = 100			# ベンチマークで検索を行う回数
srand ARGV[1].to_i

# register
N.times do
	word = random_seq()
	trie.insert word
	ftrie.insert byte_seq word
end

# retrive
trie_result = []
ftrie_result = []
list = []	# 検索用の文字列
M.times { list.push random_seq() }

Benchmark.bmbm do |x|
	# Trie
	x.report("Trie ") do
		list.each do |seq|
			trie_result.push trie.fuzzysearch(seq, cost(seq))
		end
	end
	# Ftrie
	x.report("Ftrie") do
		list.each do |seq|
			ftrie_result.push ftrie.fuzzysearch(byte_seq(seq), cost(seq))
		end
	end
end

if trie_result != ftrie_result
	puts '結果が一致していません'
	exit
end

