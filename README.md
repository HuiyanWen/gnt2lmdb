<h3>Readme</h3>
<ul>
<li>Implement in a private project, in order to convert the huge gnt dataset to lmdb pattern effeciently.</li>
<li>Environment:Caffe&cuda are needed, but you can use the dependency files I've uploaded.</li>
<li>Based on <a href="http://www.p-chao.com/2016-05-05/caffe%E7%9A%84%E5%9B%BE%E5%83%8F%E8%BD%AC%E6%8D%A2%E5%B7%A5%E5%85%B7convert_imageset%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90/">this</a></li>
</ul>
<h3>How to use</h3>
<br>gnt2lmdb.exe ./lexicon3755.txt ./hwtrn.gnt ./3755_train_RGB
<br>--shuffle=true –-resize_height=112 –-resize_width=112 --gray=false
<br>Pause
<h3>Algorithm</h3>

![Example image2](https://github.com/HuiyanWen/gnt2lmdb/blob/master/1.png)

<h3>Test dataset Netdisk</h3>
https://pan.baidu.com/s/1Nvez-5G1qMNReujazp4Nxw  code:12yt
