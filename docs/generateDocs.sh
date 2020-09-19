# This shell script will remove the ### Documentation part of the README.md file whenever generating the Doxygen documentation. 
# It will re-add them again once Doxygen has finished.

sed -i '/### Documentation/d' ../README.md
sed -i '/An extensive documentation of all classes and functions can be found \[here\](https:\/\/christianhilpert.com\/rayexec)./d' ../README.md

doxygen DoxyFileLin

echo '### Documentation' >> ../README.md
echo 'An extensive documentation of all classes and functions can be found [here](https://christianhilpert.com/rayexec).' >> ../README.md