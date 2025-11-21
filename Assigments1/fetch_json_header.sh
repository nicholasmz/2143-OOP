  #!/bin/bash
mkdir -p includes
curl -L https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp -o includes/json.hpp
echo "json.hpp downloaded successfully."
