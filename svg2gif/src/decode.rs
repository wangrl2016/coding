use std::collections::HashMap;

// 存放解码后的gif帧
pub struct FrameContainer {
    // 分离后的所有gif字符串
    splice: Vec<String>,
    // 字符串对应的下标
    // 以及(width, height, data)存放的解码数据
    // 默认保存为rgba格式
    cache: HashMap<usize, Vec<(u32, u32, Vec<u8>)>>,
}

impl FrameContainer {
    // pub fn new(splice: Vec<String>) -> Self {
    //     FrameContainer {
    //         splice,
    //         cache: HashMap::new(),
    //     }
    // }

    pub fn new() -> Self {
        FrameContainer {
            splice: vec![],
            cache: Default::default(),
        }
    }
}