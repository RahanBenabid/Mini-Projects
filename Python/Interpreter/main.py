#!/usr/bin/env python3

# Token types
#
# EOF (end-of-file) token is used to indicate that there is no more input left for lexical analisys
INTEGER, PLUS, EOF = 'INTEGER', 'PLUS', 'EOF'

class Token(object):
	def __init__(self, type, value):
		# Token types: INTEGER, PLUs, or OEF
		self.type = type
		# Token values: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, '+', or None
		self.value = value

	def __str__(self):
		"""
		String reprentation of the class intance.

		Example:
			Token(INTEGER, 3)
			Token(PLUS, '+')
		"""
		return 'Token({type}, {value})'.format(type = self.type, value = repr(self.value))

	def __repr__(self):
		return self.__str__()

class Interpreter(object):
	def __init__(self, text):
		# Client string input, example: "3+5"
		self.text = text
		# self.pos is an index into self.text
		self.pos = 0
		# current token instance
		self.current_token = None

	def error(self):
		raise Exception("Error parsing input")

	def get_next_token(self):
		"""
		Lexical analysis (also known as scanner or tokenizer

		This method is responsible for breaking a sentence apart
		into tokens. One token at a time
		"""

		text = self.text

		# is self.pos index past the end of the self.text???
		# is yes, then return EOF token because there is no more input left to convert into token
		if self.pos > len(text) - 1:
			return Token(EOF, None)

		# get character at the position slef.pos and decide what token to create based on the single character
		current_char = text[self.pos]

		# is the character is a digit then convert it into a integer, create an INTEGER token
		# increment self.pos index to point to the next character after the digit
		# then return the INTEGER token
		if current_char.isdigit():
			token = Token(INTEGER, int(current_char))
			self.pos += 1
			return token

		if current_char == '+':
			token = Token(PLUS, current_char)
			self.pos += 1
			return token

		self.error()


	def eat(self, token_type):
		# compare the current token type with the passed token type
		# if they match then "eat" the current token and assign the next token to the self.current_token
		# otherwise raise an exception
		if self.current_token.type == token_type:
			self.current_token = self.get_next_token()
		else:
			self.error()

	def expr(self):
		"""
		expr => INTEGER PUS INTEGER"""
		# set current token to the first token taken from the input
		self.current_token = self.get_next_token()

		# we expect the current token to be a single-digit integer
		left = self.current_token
		self.eat(INTEGER)

		# we expect the curret token to be a '+'
		op = self.current_token
		self.eat(PLUS)

		# now we expect the current token to be a single-digit integer
		right = self.current_token
		self.eat(INTEGER)
		# after the above call the self.current_token is set to EOF token

		# so now the sequence INTEGER PLUS INTEGER token sequence has been found
		# and  the method can just return the result of adding two integers, thus interpreting the client input
		result = left.value + right.value
		return result

def main():
	while True:
		try:
			text = input('calc> ')
		except EOFError:
			break
		if not text:
			continue
		interpreter = Interpreter(text)
		result = interpreter.expr()
		print(result)

if __name__ == "__main__":
	main()
