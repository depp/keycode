"""Common definitions."""


class Error(Exception):
    """An error that occurs during script execution.

    Attributes:
      """

    def __init__(self, msg, *, platform=None, filename=None, lineno=None):
        super(Exception, self).__init__(msg)
        self.platform = platform
        self.filename = filename
        self.lineno = lineno

    def __str__(self):
        prefix = ""
        if self.platform is not None:
            prefix += self.platform + ": "
        if self.filename is not None:
            prefix += self.filename + ": "
        if self.lineno is not None:
            prefix += "line {}: ".format(self.lineno)
        return prefix + self.args[0]
