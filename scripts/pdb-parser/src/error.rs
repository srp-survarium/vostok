use std::error;
use std::fmt;
use std::panic;

pub struct Error {
    source: Box<dyn error::Error + Send + Sync>,
    file: &'static str,
    line: u32,
}

impl Error {
    #[track_caller]
    pub fn new(error: String) -> Self {
        let loc = panic::Location::caller();

        Self {
            source: Box::new(TextError { error }),
            file: loc.file(),
            line: loc.line(),
        }
    }
}

#[macro_export]
macro_rules! bail {
    ($($arg:tt)*) =>  {
        return $crate::error!($($arg)*)
    }
}

#[macro_export]
macro_rules! error {
    ($($arg:tt)*) =>  {
        Err($crate::Error::new(format!($($arg)*)))
    };
}

impl fmt::Display for Error {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{} at '{}:{}'", self.source, self.file, self.line)
    }
}

impl fmt::Debug for Error {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        fmt::Display::fmt(self, f)
    }
}

pub type Result<T> = std::result::Result<T, Error>;

impl<E> From<E> for Error
where
    E: error::Error + Send + Sync + 'static,
{
    #[track_caller]
    fn from(err: E) -> Self {
        let loc = panic::Location::caller();
        Self {
            source: Box::new(err),
            file: loc.file(),
            line: loc.line(),
        }
    }
}

//
//
//

struct TextError {
    error: String,
}
impl error::Error for TextError {}

impl fmt::Display for TextError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "Failure: '{}'", self.error)
    }
}

impl fmt::Debug for TextError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        fmt::Display::fmt(self, f)
    }
}
