<h3>Readme</h3>
<ul>
<li>Implement in a private project, in order to convert the huge gnt dataset to lmdb pattern effeciently.</li>
<li>Environment:Caffe&cuda are needed.</li>
<li>Based on <a href="http://www.p-chao.com/2016-05-05/caffe%E7%9A%84%E5%9B%BE%E5%83%8F%E8%BD%AC%E6%8D%A2%E5%B7%A5%E5%85%B7convert_imageset%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90/">this</a></li>
</ul>
<h3>How to use</h3>
<br>
#!/usr/bin/en sh
convert_imageset --shuffle=true –resize_height=112 –resize_width=112 \
lexicon3755.txt \
hwtrn.gnt \
./3755_train_RGB\
Pause
</br>

