
import _cdtime

error = _cdtime.error
StandardCalendar = _cdtime.StandardCalendar
GregorianCalendar = _cdtime.GregorianCalendar
JulianCalendar = _cdtime.JulianCalendar
MixedCalendar = _cdtime.MixedCalendar
NoLeapCalendar = _cdtime.NoLeapCalendar
Calendar360 = _cdtime.Calendar360
ClimCalendar = _cdtime.ClimCalendar
ClimLeapCalendar = _cdtime.ClimLeapCalendar
DefaultCalendar = _cdtime.DefaultCalendar
Second = _cdtime.Second
Minutes = _cdtime.Minutes
Minute = _cdtime.Minute
Hour = _cdtime.Hour
Days = _cdtime.Days
Day = _cdtime.Day
Weeks = _cdtime.Weeks
Week = _cdtime.Week
Months = _cdtime.Months
Month = _cdtime.Month
Seasons = _cdtime.Seasons
Season = _cdtime.Season
Years = _cdtime.Years
Year = _cdtime.Year


def abstime(value, units):
    return _cdtime.abstime(value, units)


def c2r(ctype, caltype=0):
    """
    c2r(comptime, relunits[, calendar])
    """
    return _cdtime.c2r(ctype, caltype)


def compare(cdtime1, cdtime2, calendar=_cdtime.DefaultCalendar):
    "cmp( time, time [, calendar])"
    return _cdtime.compare(cdtime1, cdtime2, calendar)


def componenttime(year, month=1, day=1, hour=0, minute=0, second=0):
    return _cdtime.componenttime(year, month, day, hour, minute, second)


def comptime(year, month=1, day=1, hour=0, minute=0, second=0):
    return _cdtime.comptime(year, month, day, hour, minute, second)


def r2c(reltime, calendar=_cdtime.DefaultCalendar):
    "r2c( reltime [, calendar])"
    return _cdtime.r2c(reltime, calendar)


def r2r(reltime, newunits):
    "r2r( reltime, newunits)"
    return _cdtime.r2r(reltime, newunits)


def relativetime(value, units):
    return _cdtime.relativetime(value, units)


def reltime(value, units):
    return _cdtime.reltime(value, units)


def s2c(string, calendar=_cdtime.DefaultCalendar):
    """
    s2c(string[, calendar])
    """
    return _cdtime.s2c(string, calendar)


def s2r(ctime, runits, calendar=_cdtime.DefaultCalendar):
    """
    s2r(string, relunits [, calendar])
    """
    return _cdtime.s2r(ctime, runits, calendar)

