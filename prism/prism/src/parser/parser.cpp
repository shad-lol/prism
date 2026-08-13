#include "include/parser/parser.hpp"
#include "include/logger/logger.hpp"

#include <iostream>

prism::Parser::Parser(std::vector<Token> tokens, const CompilerConfig& compilerconfig) {
	this->tokens = tokens;
	this->compilerconfig = compilerconfig;

	while (!match(TokenType::EOFILE)) {
		if (match(TokenType::KEYWORD_ENTRY)) {			
			root.children.push_back(parseEntry());
		}
	}

	if (compilerconfig.dump_ast) {
		ErrorHandler errorhandler;
		std::stringstream ss;
		errorhandler.log(err::INFO_AST_DUMP, compilerconfig, compilerconfig.filepath, print_tree(root, ss, compilerconfig.utf8, max_depth(root)));
	}
}

std::unique_ptr<prism::Node> prism::Parser::parseEntry() {
	auto function = std::make_unique<Node>(Token(
		"function",
		TokenType::FUNC,
		std::monostate{}
	));
	function->children.push_back(std::make_unique<Node>(consume(
		TokenType::KEYWORD_ENTRY, err::SYNTAX_NO_ENTRY
	)));

	function->children.push_back(parseFunctionName());
	if (!match(TokenType::SEMICOLON)) {
		function->children.push_back(parseFunctionArguements());
		function->children.push_back(parseFunctionBody());
	}
	else pos++;

	return function;
}

std::unique_ptr<prism::Node> prism::Parser::parseFunctionName() {
	auto identifier = std::make_unique<Node>(consume(
		TokenType::IDENTIFIER, err::SYNTAX_EXPECTED_IDENTIFIER
	));
	return identifier;
}

std::unique_ptr<prism::Node> prism::Parser::parseFunctionArguements() {
	auto arguements = std::make_unique<Node>(Token(
		"arguements",
		TokenType::FUNC_ARGUEMENTS,
		std::monostate{}
	));
	
	consume(TokenType::LPAREN, err::SYNTAX_ENTRY_ISNT_FUNCTION);
	while (!match(TokenType::RPAREN)) {
		
	}
	advance();

	return arguements;
}

std::unique_ptr<prism::Node> prism::Parser::parseFunctionBody() {
	auto body = std::make_unique<Node>(Token(
		"body",
		TokenType::FUNC_BODY,
		std::monostate{}
	));

	consume(TokenType::LBRACE, err::SYNTAX_EXPECTED_SEMICOLON);
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

prism::Token prism::Parser::consume(TokenType expected, prism::err error_code) {
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

std::string prism::Parser::print_tree(const Node& node, std::stringstream& ss, bool utf8, int max_depth, std::string prefix, bool isLast, int depth) {
	if (node.value.lexeme == "root" && prefix.empty()) {
		ss << "[139, 69, 19]" << node.value.lexeme << "[]\n";
		for (size_t i = 0; i < node.children.size(); ++i) {
			bool isChildLast = (i == node.children.size() - 1);
			print_tree(*node.children[i], ss, utf8, max_depth, "", isChildLast, 1);
		}
		return ss.str();
	}

	int r = 139 + (34 - 139) * depth / max_depth;   if (r < 34)  r = 34;
	int g = 69 + (139 - 69) * depth / max_depth;    if (g > 139) g = 139;
	int b = 19 + (34 - 19) * depth / max_depth;     if (b > 34)  b = 34;

	std::string color_tag_branches = "[" + std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + "]";
	std::string color_tag_leafs = "[" + std::to_string(r + 35) + "," + std::to_string(g + 60) + "," + std::to_string(b + 15) + "]";
	std::string branch = utf8 ? (isLast ? "└── " : "├── ") : (isLast ? "`-- " : "|-- ");

	ss << prefix << color_tag_branches << branch << "[]" << color_tag_leafs << node.value.lexeme << "[]\n";

	std::string indent = utf8 ? (isLast ? "    " : "│   ") : (isLast ? "    " : "|   ");
	std::string nextPrefix = prefix + (isLast ? "    " : (color_tag_branches + indent + "[]"));

	for (size_t i = 0; i < node.children.size(); i++) {
		bool isChildLast = (i == node.children.size() - 1);
		print_tree(*node.children[i], ss, utf8, max_depth, nextPrefix, isChildLast, depth + 1);
	}

	return ss.str();
}

const prism::Node& prism::Parser::get() {
	return root;
}