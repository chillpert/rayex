# This shell script will generate the doxygen documentation.

sed -i '$d' ../README.md
sed -i '$d' ../README.md

echo '' >> ../README.md
echo '### Download' >> ../README.md
echo 'You can find the GitHub repository [here](https://github.com/chillpert/rayexec).' >> ../README.md

doxygen DoxyFileLinux

sed -i '$d' ../README.md
sed -i '$d' ../README.md
sed -i '$d' ../README.md

echo '### Documentation' >> ../README.md
echo 'An extensive documentation of all classes and functions can be found [here](https://christianhilpert.com/rayexec).' >> ../README.md