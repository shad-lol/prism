#include "include/parser/parser.hpp"
#include "include/logger/logger.hpp"

#include <iostream>

prism::Parser::Parser(std::vector<Token> tokens, CompilerConfig compilerconfig) {
	this->tokens = tokens;

	while (!match(TokenType::EOFILE)) {
		if (match(TokenType::KEYWORD_ENTRY)) {			
			root.children.push_back(parseEntry(compilerconfig));
		}
	}

	if (compilerconfig.dump_ast) {
		ErrorHandler errorhandler;
		errorhandler.log(err::INFO_AST_DUMP, compilerconfig, compilerconfig.filepath);
		print_tree(root, compilerconfig.utf8, max_depth(root), compilerconfig);
	}
}

std::unique_ptr<prism::Node> prism::Parser::parseEntry(CompilerConfig compilerconfig) {
	auto entry = std::make_unique<Node>(consume(
		TokenType::KEYWORD_ENTRY, err::SYNTAX_NO_ENTRY, compilerconfig
	));

	entry->children.push_back(parseFunctionName(compilerconfig));
	if (!match(TokenType::SEMICOLON)) {
		entry->children.push_back(parseFunctionArguements(compilerconfig));
		entry->children.push_back(parseFunctionBody(compilerconfig));
	}
	else pos++;

	return entry;
}

std::unique_ptr<prism::Node> prism::Parser::parseFunctionName(CompilerConfig compilerconfig) {
	auto identifier = std::make_unique<Node>(consume(
		TokenType::IDENTIFIER, err::SYNTAX_EXPECTED_IDENTIFIER, compilerconfig
	));
	return identifier;
}

std::unique_ptr<prism::Node> prism::Parser::parseFunctionArguements(CompilerConfig compilerconfig) {
	auto arguements = std::make_unique<Node>(Token(
		"arguements",
		TokenType::FUNC_ARGUEMENTS,
		std::monostate{}
	));
	
	consume(TokenType::LPAREN, err::SYNTAX_ENTRY_ISNT_FUNCTION, compilerconfig);
	while (!match(TokenType::RPAREN)) {
		
	}
	advance();

	return arguements;
}

std::unique_ptr<prism::Node> prism::Parser::parseFunctionBody(CompilerConfig compilerconfig) {
	auto body = std::make_unique<Node>(Token(
		"body",
		TokenType::FUNC_BODY,
		std::monostate{}
	));

	consume(TokenType::LBRACE, err::SYNTAX_EXPECTED_SEMICOLON, compilerconfig);
	while (!match(TokenType::RBRACE)) {
		if (match(TokenType::KEYWORD_RETURN)) {
			auto ret = std::make_unique<Node>(pop());
			ret->children.push_back(parseReturnValue());
			body->children.push_back(std::move(ret));
		}
	}
	advance();

	return body;
}

std::unique_ptr<prism::Node> prism::Parser::parseReturnValue() {
	std::unique_ptr<Node> value;

	while (!match(TokenType::SEMICOLON)) {
		if (match(TokenType::LITERAL_INT))    value = std::make_unique<Node>(pop());
		if (match(TokenType::LITERAL_LL))     value = std::make_unique<Node>(pop());
		if (match(TokenType::LITERAL_FLOAT))  value = std::make_unique<Node>(pop());
		if (match(TokenType::LITERAL_DOUBLE)) value = std::make_unique<Node>(pop());
	}
	advance();

	return value;
}

bool prism::Parser::match(TokenType expected) {
	if (tokens[pos].type == expected) return true;
	return false;
}

prism::Token prism::Parser::consume(TokenType expected, prism::err error_code, CompilerConfig compilerconfig) {
	if (tokens[pos].type != expected) {
		prism::ErrorHandler errorhandler;
		errorhandler.log(error_code, compilerconfig);
	}
	return tokens[pos++];
}

int prism::Parser::max_depth(const Node& node, int depth) {
	int mx = depth;
	for (const auto& child : node.children) {
		int child_d = max_depth(*child, depth + 1);
		if (child_d > mx) mx = child_d;
	}
	return mx;
}

void prism::Parser::print_tree(const Node& node, bool utf8, int max_depth, CompilerConfig compilerconfig, std::string prefix, bool isLast, int depth) {
	Logger logger;

	if (node.value.lexeme == "root" && prefix.empty()) {
		logger.coutrgb(node.value.lexeme + "\n", compilerconfig.ansi);
		for (size_t i = 0; i < node.children.size(); ++i) {
			bool isChildLast = (i == node.children.size() - 1);
			print_tree(*node.children[i], utf8, max_depth, compilerconfig, "", isChildLast, 1);
		}
		return;
	}

	int r = 139 + (34 - 139) * depth / max_depth;   if (r < 34)  r = 34;
	int g = 69 + (139 - 69) * depth / max_depth;    if (g > 139) g = 139;
	int b = 19 + (34 - 19) * depth / max_depth;     if (b > 34)  b = 34;

	std::string color_tag = "[" + std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + "]";
	std::string branch = utf8 ? (isLast ? "└── " : "├── ") : (isLast ? "`-- " : "|-- ");

	logger.coutrgb(prefix + color_tag + branch + "[]" + node.value.lexeme + "\n", compilerconfig.ansi);

	std::string indent = utf8 ? (isLast ? "    " : "│   ") : (isLast ? "    " : "|   ");
	std::string nextPrefix = prefix + (isLast ? "    " : (color_tag + indent + "[]"));

	for (size_t i = 0; i < node.children.size(); ++i) {
		bool isChildLast = (i == node.children.size() - 1);
		print_tree(*node.children[i], utf8, max_depth, compilerconfig, nextPrefix, isChildLast, depth + 1);
	}
}

const prism::Node& prism::Parser::get() {
	return prism::Parser::root;
}