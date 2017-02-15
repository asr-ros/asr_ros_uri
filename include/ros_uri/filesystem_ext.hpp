/**

Copyright (c) 2016, Schleicher Ralf
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef FILESYSTEM_EXT_HPP_
#define FILESYSTEM_EXT_HPP_

#include <boost/filesystem.hpp>

namespace boost {
	namespace filesystem {
		class pathext {
		public:
			/**
			 * @param first the first param to compare
			 * @param second the second param to compare
			 * @return the common prefix
			 */
			static path common_prefix(path& first, path& second) {
				// the resulting string.
				path result;

				// if it's a root path, append the "/" sign to the empty result
				if (first.string().find("/") == 0) {
					result /= "";
				}

				// get the first mismatching iterator
				std::pair<path::iterator, path::iterator> iteratorPair = std::mismatch(first.begin(), first.end(), second.begin());
				for (path::iterator iter = first.begin(); iter != iteratorPair.first; iter++) {
					result /= *iter;
				}

				return result;
			};

			/**
			 * @param subject the subject to get the relative path
			 * @param base the base path
			 * @return the path
			 */
			static path relative_path(path& subject, path base = current_path()) {
				path result;

				std::pair<path::iterator, path::iterator> iteratorPair = std::mismatch(base.begin(), base.end(), subject.begin());
				for (path::iterator iter = iteratorPair.second; iter != subject.end(); iter++) {
					path subpath = *iter;

					if (subpath.string() == ".") {
						continue;
					}

					result /= subpath;
				}

				return result;
			};
		};
	}
}


#endif /* FILESYSTEM_EXT_HPP_ */
