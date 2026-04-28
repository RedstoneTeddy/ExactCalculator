
import random



def GenerateNumber(limit: int = 1000, non_negative: bool = False) -> tuple[str, float]:
    if non_negative:
        number: float = random.random() * limit
    else:
        number = random.random() * random.randint(-limit, limit)
    number_str: str = str(number)
    if number_str.startswith("-"):
        number_str = number_str.replace("-", "_", 1)
    return number_str, number

def GenerateInteger(limit: int = 1000, non_negative: bool = False) -> tuple[str, int]:
    if non_negative:
        integer: int = random.randint(0, limit)
    else:
        integer = random.randint(-limit, limit)
    integer_str: str = str(integer)
    if integer_str.startswith("-"):
        integer_str = integer_str.replace("-", "_", 1)
    return integer_str, integer