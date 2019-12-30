### Readme
<ul>
<li>Implement in a private project, in order to convert the huge gnt dataset to lmdb pattern effeciently.</li>
<li>Environment:Caffe&cuda are needed, but you can use the dependency files I've uploaded.</li>
<li>Based on <a href="http://www.p-chao.com/2016-05-05/caffe%E7%9A%84%E5%9B%BE%E5%83%8F%E8%BD%AC%E6%8D%A2%E5%B7%A5%E5%85%B7convert_imageset%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90/">this</a></li>
</ul>

### How to use
You can copy the create_lmdb.bat into the dependency files' directory and click it,  its' parameters as follows:
```
gnt2lmdb.exe ./lexicon3755.txt ./hwtrn.gnt ./3755_train_RGB\
--shuffle=true –-resize_height=112 –-resize_width=112 --gray=false
Pause
```

### Algorithm

![Example image2](https://github.com/HuiyanWen/gnt2lmdb/blob/master/1.png)

<h3>Netdisk</h3>
<br>Test dataset:https://pan.baidu.com/s/1Nvez-5G1qMNReujazp4Nxw  code:12yt
<br>Dependency files:https://pan.baidu.com/s/13s7LXVzyREAweBWBWAZTLA  code:e3m2
