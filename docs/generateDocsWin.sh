# This shell script will generate the doxygen documentation.
sed -i '$d' ../README.md
sed -i '$d' ../README.md

echo '' >> ../README.md
echo '## Download' >> ../README.md
echo 'You can find the GitHub repository [here](https://github.com/chillpert/rayex).' >> ../README.md

# Generate doxygen documentation.
doxygen DoxyFileLinux

sed -i '$d' ../README.md
sed -i '$d' ../README.md
sed -i '$d' ../README.md

echo '### Documentation' >> ../README.md
echo 'An extensive documentation of all classes and functions can be found [here](https://chillpert.github.io/rayex/html/index.html).' >> ../README.md

# Convert to markdown.
moxygen xml -g -q --output moxygen/api-%s.md
