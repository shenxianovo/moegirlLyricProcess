## txt复制后格式：
### 1
あいうえお[1]漢字（漢字）（）中文翻译
（） // 换行符
かきくけこ （）翻译

阅读更多：.....
.......

### 2
あいうえお[1]漢字（漢字）中文翻译
无换行符

## HTML源代码格式
歌词部分：
```HTML
    <p><small>本段落中所使用的歌词，其著作权属于原著作权人，仅以介绍为目的引用。</small>
    <style data-mw-deduplicate=" ">  </style>
        <div class="Lyrics Lyrics-has-ruby Lyrics-has-translate" style>
            <!-- 处理单位为lyrics-line 分为original与translated -->
            <div class="Lyrics-line">
                <div class="Lyrics-original" style=" ">
                    <span lang="ja">
                    あいうえお
                        <ruby class="photrans">
                            <rb>漢字</rb>
                            <rt style=" ">
                                <span class=" ">（ </span> <!-- 括号 -->
                                    かんじ
                                <span class=" ">（ </span>
                            </rt>
                        </ruby>
                    かきくけこ
                    <ruby class="photrans">埋　</ruby>
                    まれ
                    </span>
                </div>
                <div class="Lyrics-translated" style> </div>

```


## 处理后(formated)格式
### 中日歌词 + 注音 + 罗马字
**千本桜**
| 层级  |         内容          |
| :---: | :-------------------: |
|   4   |         中文          |
|   6   |  假名 + romaji右括号  |
|   7   | romaji + romaji左括号 |
|   8   |      汉字 + 注音      |
|   9   |       假名括号        |

### 中日歌词 + 注音
**INTERNET OVERDOSE**
| 层级  |     内容     |
| :---: | :----------: |
|   4   | 假名 + 中文  |
|   6   | 汉字 + 注音  |
|   7   | 假名左右括号 |

**メフィスト**
| 层级  |     内容     |
| :---: | :----------: |
|   4   | 假名 + 中文  |
|   6   | 汉字 + 注音  |
|   7   | 假名左右括号 |

**雑踏、僕らの街**
(整句假名注音在末尾) _啊啊啊啊啊啊啊啊啊啊_
| 层级  | 内容  |
| :---: | :---: |
|   4   | 中文  |
|   6   | 假名  |
|   7   | 汉字+注音-汉字  |
|   8   | 注音-假名  |

#### 中日歌词
**空の箱**
| 层级  | 内容  |
| :---: | :---: |
|   4   | 日语  |
|   4   | 中文  |

**栞**
| 层级  | 内容  |
| :---: | :---: |
|   5   | 日语  |
|   4   | 中文  |