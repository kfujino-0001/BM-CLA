# -----------------------------------------------------------------------------
# HTM Community Edition of NuPIC
# Copyright (C) 2016, Numenta, Inc.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero Public License version 3 as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU Affero Public License for more details.
#
# You should have received a copy of the GNU Affero Public License
# along with this program.  If not, see http://www.gnu.org/licenses.
# -----------------------------------------------------------------------------

# Fetch Nlohmann from GitHub archive
#
if(EXISTS "${REPOSITORY_DIR}/build/ThirdParty/share/json-develop.zip")
    set(URL "${REPOSITORY_DIR}/build/ThirdParty/share/json-develop.zip")
else()
	set(URL "https://github.com/nlohmann/json/archive/refs/heads/develop.zip")
endif()

message(STATUS "obtaining Nlohmann")
include(DownloadProject/DownloadProject.cmake)
download_project(PROJ nlohmann
	PREFIX ${EP_BASE}/nlohmann
	URL ${URL}
	UPDATE_DISCONNECTED 1
	QUIET
	)
	
# No build. This is a header only package


FILE(APPEND "${EXPORT_FILE_NAME}" "nlohmann_INCLUDE_DIRS@@@${nlohmann_SOURCE_DIR}\n")