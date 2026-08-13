#pragma once

#include <string>

namespace prism {

	enum PIRinstruction {
		FUNC_START,
		FUNC_END,
		ARG_START,
		ARG_END,
		BODY_START,
		BODY_END,

		ENTRY,

		IDENTIF,

		INT,
		LL,
		FLOAT,
		DOUBLE,

		RET,
	};

}