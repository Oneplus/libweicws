#define PCRE_STATIC
#include <iostream>
#include "pcrecpp.h"

using namespace std;

int main() {
    string sent = "[音乐] #我正在听# @MCHOTDOG熱狗《差不多先生》 http://t.cn/h0VJQ  （分享自 @微博音乐盒）";
	pcrecpp::StringPiece input(sent);
	pcrecpp::RE pattern("(https?|ftp|file):"
            "//[-A-Za-z0-9+&@#/%?=~_|!:,.;]*"
            "[-A-Za-z0-9+&@#/%=~_|]");

	while (pattern.Consume(&input)) {
	}

	return 0;
}
