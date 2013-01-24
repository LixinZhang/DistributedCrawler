echo -n 'You have coded lines : '
cat *.cpp fetch/*.h fetch/*.cpp filter/*.h filter/*.cpp urlFrontier/*h urlFrontier/*.cpp consistentHash/*.h consistentHash/*.cpp distributed/*.h distributed/*.cpp | sed '/^$\|\/\/\|\/\*|\*\//d' | wc -l
echo -n 'You have coded lines with comments : ' 
cat *.cpp fetch/*.h fetch/*.cpp filter/*.h filter/*.cpp urlFrontier/*h urlFrontier/*.cpp consistentHash/*.h consistentHash/*.cpp  distributed/*.h distributed/*.cpp | wc -l
