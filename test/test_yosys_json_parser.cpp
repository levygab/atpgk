#include <vector>
#include <map>
#include <fstream>

#include <gtest/gtest.h>

#include "../include/parser/yosys_json_parser.hpp"

// Test fixture for gate definition error
TEST(YosysJSONParser, GateDefinitionErrorTest) {

    json strings;

    // Processing string management
    std::ifstream strings_file;

    std::locale current_locale("");
    std::string local_language = current_locale.name().substr(0,2);

    // FIXME: Problème de spécification statique du fichier d'entrée
    // Impossible d'éxecuter le programme ailleurs que depuis son propre dossier
    if (std::filesystem::exists("../lang/"+local_language+".json")) {
        strings_file.open("../lang/"+local_language+".json");
    } else {
        strings_file.open("../lang/en.json");
    }

    std::string fileString = R"(
    {
        "modules": {
            "comb": {
                "ports": {
                    "a": {
                        "direction": "input",
                        "bits": [ 2 ]
                    },
                    "b": {
                        "direction": "input",
                        "bits": [ 3 ]
                    },
                    "c": {
                        "direction": "output",
                        "bits": [ 4 ]
                    }
                },
                "cells": {
                    "AND": {
                        "type": "$_AD_",
                        "port_directions": {
                            "A": "input",
                            "B": "input",
                            "Y": "output"
                        },
                        "connections": {
                            "A": [ 3 ],
                            "B": [ 2 ],
                            "Y": [ 4 ]
                        }
                    }
                },
                "netnames": {
                    "c": {
                        "bits": [ 4 ]
                    },
                    "a": {
                        "bits": [ 2 ]
                    },
                    "b": {
                        "bits": [ 3 ]
                    }
                }
            }
        }
    })";
    
    YosysJSONParser parser(strings);
    parser.setInputFileContent(fileString);
    ASSERT_EXIT(
        parser.parseCircuit(),
        testing::ExitedWithCode(1),
        ""
    );
}

// Test fixture for port definition error
TEST(YosysJSONParser, PortDefinitionError) {

    json strings;

    // Processing string management
    std::ifstream strings_file;

    std::locale current_locale("");
    std::string local_language = current_locale.name().substr(0,2);

    // FIXME: Problème de spécification statique du fichier d'entrée
    // Impossible d'éxecuter le programme ailleurs que depuis son propre dossier
    if (std::filesystem::exists("../lang/"+local_language+".json")) {
        strings_file.open("../lang/"+local_language+".json");
    } else {
        strings_file.open("../lang/en.json");
    }

    std::string fileString = R"(
    {
        "modules": {
            "comb": {
                "ports": {
                    "a": {
                        "direction": "input",
                        "bits": [ 2 ]
                    },
                    "b": {
                        "direction": "input",
                        "bits": [ 3 ]
                    },
                    "c": {
                        "direction": "tput",
                        "bits": [ 4 ]
                    }
                },
                "cells": {
                    "AND": {
                        "type": "$_AND_",
                        "port_directions": {
                            "A": "input",
                            "B": "input",
                            "Y": "output"
                        },
                        "connections": {
                            "A": [ 3 ],
                            "B": [ 2 ],
                            "Y": [ 4 ]
                        }
                    }
                },
                "netnames": {
                    "c": {
                        "bits": [ 4 ]
                    },
                    "a": {
                        "bits": [ 2 ]
                    },
                    "b": {
                        "bits": [ 3 ]
                    }
                }
            }
        }
    })";
    
    YosysJSONParser parser(strings);
    parser.setInputFileContent(fileString);

    ASSERT_EXIT(
        parser.parseCircuit(),
        testing::ExitedWithCode(1),
        ""
    );
}

// Test fixture for parsing content
TEST(YosysJSONParser, ParsingTest) {

    json strings;

    // Processing string management
    std::ifstream strings_file;

    std::locale current_locale("");
    std::string local_language = current_locale.name().substr(0,2);

    // FIXME: Problème de spécification statique du fichier d'entrée
    // Impossible d'éxecuter le programme ailleurs que depuis son propre dossier
    if (std::filesystem::exists("../lang/"+local_language+".json")) {
        strings_file.open("../lang/"+local_language+".json");
    } else {
        strings_file.open("../lang/en.json");
    }

    std::string fileString = R"(
    {
        "modules": {
            "comb": {
                "ports": {
                    "a": {
                        "direction": "input",
                        "bits": [ 2 ]
                    },
                    "b": {
                        "direction": "input",
                        "bits": [ 3 ]
                    },
                    "c": {
                        "direction": "output",
                        "bits": [ 4 ]
                    }
                },
                "cells": {
                    "AND": {
                        "type": "$_AND_",
                        "port_directions": {
                            "A": "input",
                            "B": "input",
                            "Y": "output"
                        },
                        "connections": {
                            "A": [ 3 ],
                            "B": [ 2 ],
                            "Y": [ 4 ]
                        }
                    }
                },
                "netnames": {
                    "c": {
                        "bits": [ 4 ]
                    },
                    "a": {
                        "bits": [ 2 ]
                    },
                    "b": {
                        "bits": [ 3 ]
                    }
                }
            }
        }
    })";

    std::vector<uint> expected_input_vector = {2,3};
    std::vector<uint> expected_output_vector = {4};
    std::vector<uint> expected_wire_vector = {2,3,4};

    YosysJSONParser parser(strings);
    parser.setInputFileContent(fileString);

    ParsedCircuit netlist = parser.parseCircuit();

    ASSERT_EQ(netlist.input_vector, expected_input_vector);
    ASSERT_EQ(netlist.output_vector, expected_output_vector);
    ASSERT_EQ(netlist.wire_vector, expected_wire_vector);
}