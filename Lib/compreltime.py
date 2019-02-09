
from . import _cdtime

Calendar360 = _cdtime.Calendar360
ClimCalendar = _cdtime.ClimCalendar
ClimLeapCalendar = _cdtime.ClimLeapCalendar
Day = _cdtime.Day
Days = _cdtime.Days
DefaultCalendar = _cdtime.DefaultCalendar
GregorianCalendar = _cdtime.GregorianCalendar
Hour = _cdtime.Hour
Hours = _cdtime.Hours
JulianCalendar = _cdtime.JulianCalendar
Minute = _cdtime.Minute
Minutes = _cdtime.Minutes
MixedCalendar = _cdtime.MixedCalendar
Month = _cdtime.Month
Months = _cdtime.Months
NoLeapCalendar = _cdtime.NoLeapCalendar
Season = _cdtime.Season
Seasons = _cdtime.Seasons
Second = _cdtime.Second
Seconds = _cdtime.Seconds
StandardCalendar = _cdtime.StandardCalendar
Week = _cdtime.Week
Weeks = _cdtime.Weeks
Year = _cdtime.Year
Years = _cdtime.Years


error = _cdtime.error


def abstime(value, units):
    return _cdtime.abstime(value, units)


def c2r(ctype, newunits):
    """
        c2r(comptime, relunits[, calendar])

        Parameters
        ----------

            ctype:  component time class object
            newunits: [seconds,hours,days,yeears] since [year]

        Returns
        -------

           Relative time class object.
        ::

            >>> myComponentTime = cdtime.comptime(2020,1,10)
            >>> myRelativeTime = cdtime.c2r(myComponentTime,"days since 2020")
            >>> myRelativeTime
            9.00 days since 2020

    """
    return _cdtime.c2r(ctype, newunits)


def compare(cdtime1, cdtime2, calendar=_cdtime.DefaultCalendar):
    """
        compare time object

            cmp(time1, time2[, calendar])

        Parameters
        ----------

            time1:  component or relative time class object
            time2:  component or relative time class object
            calendar: [default to Gregorian]

        Returns
        -------

           -1:  if time1 < time2
            0: if time == time2
            1: if time1 > time2
        ::

            >>> myComponentTime = cdtime.comptime(2020,1,10)
            >>> myRelativeTime = cdtime.relativetime(11,"days since 2020")
            >>> cdtime.cmp(myComponentTime, myRelativeTime, cdtime.GregorianCalendar)
            >>> cdtime.compare(myRelativeTime, myComponentTime, cdtime.GregorianCalendar)
            1
            >>> cdtime.compare(myComponentTime, myRelativeTime, cdtime.GregorianCalendar)
            -1

    """
    return _cdtime.compare(cdtime1, cdtime2, calendar)


def componenttime(year, month=1, day=1, hour=0, minute=0, second=0):
    """
        Create a component time object.

            componenttime(year, month, day, hour, minute, second)

        Parameters
        ----------
            year:
            month:  default = 1
            day: default = 1
            hour: default = 0
            minute: default = 0
            second: default = 0

        Returns
        -------
            Component Time class object

        ::
            >>> timeStart = cdtime.comptime(2017)
            >>> timeEnd = cdtime.comptime(2017,12,31,23,59,59)
            >>> timeStart
            2017-1-1 0:0:0.0
            >>> timeEnd
            2017-12-31 23:59:59.0

    """
    return _cdtime.componenttime(year, month, day, hour, minute, second)


def comptime(year, month=1, day=1, hour=0, minute=0, second=0):
    """
        Create a component time object.

            componenttime(year, month, day, hour, minute, second)

        Parameters
        ----------
            year:
            month:  default = 1
            day: default = 1
            hour: default = 0
            minute: default = 0
            second: default = 0

        Returns
        -------
            Component Time class object

        ::
            >>> timeStart = cdtime.comptime(2017)
            >>> timeEnd = cdtime.comptime(2017,12,31,23,59,59)
            >>> timeStart
            2017-1-1 0:0:0.0
            >>> timeEnd
            2017-12-31 23:59:59.0

    """

    return _cdtime.comptime(year, month, day, hour, minute, second)


def r2c(reltime, calendar=_cdtime.GregorianCalendar):
    """
        Convert Relative time object into component time object.
                r2c( reltime [, calendar])

        Parameters
        ----------

            reltime:  Relative time object
            calendar:  Default is Gregorian

        Returns
        -------

            A component time object

        ::
            >>> import cdtime
            >>> timeStart = cdtime.reltime(9, "days since 2008")
            >>> cdtime.r2c(timeStart, cdtime.GregorianCalendar)
            2008-1-10 0:0:0.0
    """
    return _cdtime.r2c(reltime, calendar)


def r2r(reltime, newunits):
    """
        Convert Relative time object to newunits.
                r2r( reltime, newunits )

        Parameters
        ----------

            reltime:  Relative time object
            newunits:  [seconds,hours,days,yeears] since [year]

        Returns
        -------

            A relative time object

        ::
            >>> import cdtime
            >>> timeStart = cdtime.reltime(9, "days since 2008")
            >>> cdtime.r2r(timeStart, "days since 2007")
            374.000000 days since 2007
    """
    return _cdtime.r2r(reltime, newunits)


def relativetime(value, units):
    """
        Create a retlative time object.

            relativetime(value, units)

        Parameters
        ----------
            values:  number (can be floating point)
            units:  [seconds,hours,days,yeears] since [year]

        Returns
        -------
            Relative Time class object

        ::
            >>> timeStart = cdtime.reltime(9, "days since 2008")
            >>> timeStart
            9.000000 days since 2008

    """
    return _cdtime.relativetime(value, units)


def reltime(value, units):
    """
        Create a retlative time object.

            relativetime(value, units)

        Parameters
        ----------
            values:  number (can be floating point)
            units:  [seconds,hours,days,yeears] since [year]

        Returns
        -------
            Relative Time class object

        ::
            >>> timeStart = cdtime.reltime(9, "days since 2008")
            >>> timeStart
            9.000000 days since 2008

    """
    return _cdtime.reltime(value, units)


def s2c(string, calendar=_cdtime.GregorianCalendar):
    """
        Convert a string to a component time object
            s2c(string[, calendar])

        Parameters
        ----------
            string: date string in form "YYYY-MM-DD HH:MM:SS.Milisecond
            calendar:  Default to Gregorian Calendar

        Returns
        -------

            Component time object

        ::
            >>> import cdtime
            >>> startTime = cdtime.s2c("2008-10-01 23:59:59")
            >>> startTime
            2008-10-1 23:59:59.0
            >>> type(startTime)
            comptime

    """
    return _cdtime.s2c(string, calendar)


def s2r(ctime, runits, calendar=_cdtime.DefaultCalendar):
    """
        Convert a string to a relative time object
            s2r(string, relunits [, calendar])

        Parameters
        ----------
            string: date string in form "YYYY-MM-DD HH:MM:SS.Milisecond
            calendar:  Default to Gregorian Calendar

        Returns
        -------
            Relative time object

        ::
            >>> import cdtime
            >>> startTime = cdtime.s2r("2008-10-01 23:59:59","days since 2008")
            >>> startTime
            274.999988 days since 2008
            >>> type(startTime)
            reltime


    """
    return _cdtime.s2r(ctime, runits, calendar)
