use crate::advance::*;
use crate::utils::util::*;

use self::function_programming::*;
use self::global_variable::*;
use self::handle_error::*;
use self::lifetime::*;
use self::macro_programming::*;
use self::muti_thread::*;
use self::reference::*;
use self::smart_pointer::*;
use self::types::*;
use self::unsafe_code::*;

pub fn run() {
    // show_chapter("函数式编程");
    // function_programming::exec();

    // show_chapter("全局变量");
    // global_variable::exec();

    // show_chapter("错误处理");
    // handle_error::exec();

    // show_chapter("声明周期");
    // lifetime::exec();

    // show_chapter("宏编程");
    // macro_programming::exec();

    // show_chapter("多线程");
    // muti_thread::exec();

    show_chapter("智能指针");
    smart_pointer::exec();

    // show_chapter("类型");
    // types::exec();

    // show_chapter("unsafe");
    // unsafe_code::exec();
}
