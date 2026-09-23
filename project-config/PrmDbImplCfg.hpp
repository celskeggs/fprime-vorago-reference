// Copyright 2026 California Institute of Technology
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*
 * PrmDblImplCfg.hpp
 *
 *  Created on: Mar 13, 2015
 *      Author: tcanham
 */

#ifndef PRMDB_PRMDBLIMPLCFG_HPP_
#define PRMDB_PRMDBLIMPLCFG_HPP_

// Anonymous namespace for configuration parameters
namespace {

enum {
    PRMDB_NUM_DB_ENTRIES = 1,     // !< Number of entries in the parameter database
    PRMDB_ENTRY_DELIMITER = 0xA5  // !< Byte value that should precede each parameter in file; sanity check against
                                  // file integrity. Should match ground system.
};

}

#endif /* PRMDB_PRMDBLIMPLCFG_HPP_ */
