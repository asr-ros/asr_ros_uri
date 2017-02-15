/**

Copyright (c) 2016, Schleicher Ralf
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef ROS_URI_HPP_
#define ROS_URI_HPP_

#include <vector>
#include <string>
#include <ros/ros.h>
#include <ros/package.h>
#include "ros_uri/filesystem_ext.hpp"

namespace fs = boost::filesystem;

/**
 * The ros_uri class delivers functionality equally to resource_finder.
 *
 * With one exception. It doesn't load the resource into memory but just delivers the uri path to a given file
 * or a file path for a given uri.
 *
 * They should have done it in resource_finder. They didn't, so I did.
 *
 * @author Ralf Schleicher
 */
class ros_uri {
private:
	/**
	 * @return the prefix for the package protocol
	 */
	inline static const std::string getPackagePrefix() {
		return "package://";
	};

	/**
	 * @return the prefix for the file protocol
	 */
	inline static const std::string getFilePrefix() {
		return "file://";
	};
public:
	/**
	 * Converts a protocol uri to an absolute path.
	 * @param uriString - string containing the uri
	 * @return a string containing the path to the referenced uri.
	 */
	static std::string absolute_path(std::string uriString) {
		if (uriString.find(getPackagePrefix()) == 0) {
			std::size_t pos = uriString.find("/", getPackagePrefix().size());
			std::string packageName = uriString.substr(getPackagePrefix().size(), pos - getPackagePrefix().size());
			std::string packagePathStr = ros::package::getPath(packageName);
			return packagePathStr + uriString.substr(pos, uriString.size() - pos);
		} else if (uriString.find(getFilePrefix()) == 0) {
			std::string pathStr = uriString.substr(getFilePrefix().size(), uriString.size() - getFilePrefix().size());
			return pathStr;
		}
		return "";
	};

	/**
	 * Converts a path to a package-protocol uri.
	 * @param pathStr - the absolute path
	 * @param packageName (optional, default: "") - the name of the package the path lies into.
	 * @return a string in valid uri format prefixed with package-protocl name
	 */
	static std::string package_uri(std::string pathStr, std::string packageName = std::string()) {
		// Get the abs path
		fs::path absPath = fs::path(pathStr);

		// if there is no package name given, start searching.
		if (packageName == std::string()) {
			// get all possible packaged
			std::vector<std::string> packageVector;
			ros::package::getAll(packageVector);

			// do a stable sort, so we can do the binary search
			std::stable_sort(packageVector.begin(), packageVector.end());

			// start with the current path and then get step by step up the path
			fs::path parentPath = absPath;

			// as long as there is a parent do the search process.
			while (parentPath != fs::path()) {
				// get the name of the file
				fs::path filename = parentPath.filename();

				// do the binary search for the package, if it's found
				if (std::binary_search(packageVector.begin(), packageVector.end(), filename)) {
					// get the path to package, and if it equal to the rest of the path nothing to do anymore
					fs::path possiblePath = fs::path(ros::package::getPath(filename.string()));
					if (possiblePath == parentPath.string()) {
						packageName = filename.string();
						// stop looping
						break;
					}
				}

				parentPath = parentPath.parent_path();
			}
		}

		// create the uri
		fs::path baseFolder = ros::package::getPath(packageName);
		fs::path relativePath = fs::pathext::relative_path(absPath, baseFolder);
		return getPackagePrefix() + packageName + "/" + relativePath.string();
	};

	/**
	 * simply prefixes the path with the file-protocol prefix.
	 */
	static std::string file_uri(fs::path path) {
		return getFilePrefix() + fs::absolute(path).string();
	};
};


#endif /* ROS_URI_HPP_ */
