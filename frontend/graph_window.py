import pygame

import frontend.graph_calculation
import time
import math

class GraphWindow:
    def __init__(self) -> None:
        self.width: int = 800
        self.height: int = 600
        self.screen: pygame.Surface | None = None
        self.last_frame: pygame.Surface | None = None
        self.clock = pygame.time.Clock()

        self.zoom: int = 5
        self.x_size: tuple[float, float] = (-10.0, 10.0)
        self.y_size: tuple[float, float] = (-10.0, 10.0)

        # Store calculated points to reduce recalculations on resize / moving
        self.x_values: list[float | None] = [] # X are all the same for all functions
        self.y_values: list[dict[float, float | None]] = []
        self.functions: list[str] = []

        self.significance: int = 10
        self.timeout: float = 10.0

        self.time_calculation: float = 0.0
        self.time_preparation: float = 0.0
        self.time_drawing: float = 0.0

        self.moved: bool = False


    def Resize_check(self) -> None:
        new_size: tuple[int, int] = pygame.display.get_window_size()
        if new_size != (self.width, self.height):
            old_width: int = self.width
            old_height: int = self.height
            center_x: float = (self.x_size[0] + self.x_size[1]) / 2
            center_y: float = (self.y_size[0] + self.y_size[1]) / 2

            x_scale: float = (self.x_size[1] - self.x_size[0]) / old_width if old_width != 0 else 0.0
            y_scale: float = (self.y_size[1] - self.y_size[0]) / old_height if old_height != 0 else 0.0
            if x_scale > 0 and y_scale > 0:
                scale: float = (x_scale + y_scale) / 2
            else:
                scale = x_scale if x_scale > 0 else y_scale if y_scale > 0 else 1.0

            self.width, self.height = new_size
            self.x_size = (center_x - scale * self.width / 2, center_x + scale * self.width / 2)
            self.y_size = (center_y - scale * self.height / 2, center_y + scale * self.height / 2)

            if self.screen is not None:
                self.screen = pygame.display.set_mode((self.width, self.height), pygame.RESIZABLE)

            self.Prepare_graph_drawing()




    def Load_function(self, functions: str, initialZoom: int) -> None:
        self.x_values = []
        self.y_values = []
        self.functions = []
        functions = functions.replace(" ", "")
        for func in functions.split(";"):
            if func != "":
                self.functions.append(func)
                self.y_values.append({})
        self.zoom = initialZoom
        self.x_size = (-self.width//10/10, self.width//10/10)
        self.y_size = (-self.height//10/10, self.height//10/10)
        self.Prepare_graph_drawing()

        self.screen = pygame.display.set_mode((self.width, self.height), pygame.RESIZABLE)
        pygame.display.set_caption("Graph Window")

    def start(self) -> None:
        running: bool = True
        pygame.init()
        while running:
            self.Resize_check()

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False


            if self.screen is None:
                continue
            self.screen.fill((255, 255, 255)) # Clear the screen with white background

            self.Check_for_moving()

            self.Draw_graph(self.x_size, self.y_size, 1 / 2 ** self.zoom)

            if self.screen is not None:
                self.last_frame = self.screen.copy()



            pygame.display.flip()
            self.clock.tick(60) # Limit to 60 FPS

        pygame.quit()



    def Calculate_points(self, x_size: tuple[float, float], zoom: int) -> None:
        # Calculation Logic
        x_to_calculate: list[float] = []

        point_distance: float = 1 / 2 ** zoom


        start_time: float = time.time()
        for x_i in range(int(x_size[0] / point_distance), int(x_size[1] / point_distance) + 1, 1):
            x_value: float = x_i * point_distance
            if x_value not in self.x_values:
                x_to_calculate.append(x_value)
        end_time: float = time.time()
        self.time_preparation = end_time - start_time

        if len(x_to_calculate) == 0:
            return
        elif len(x_to_calculate) > 100:
            self.Draw_calculating_overlay()

        # Calculate all y values
        start_time = time.time()
        y_new: list[list[float | None]] = frontend.graph_calculation.Calculate_graphs(self.functions, timeout=self.timeout, x_values=x_to_calculate, significance=self.significance)
        end_time = time.time()
        self.time_calculation = end_time - start_time

        # Sort new x and new y into old lists
        for x_i in range(len(x_to_calculate)):
            x_value = x_to_calculate[x_i]
            self.x_values.append(x_value)

            for func_i in range(len(self.functions)):
                y_value: float | None = y_new[func_i][x_i]
                self.y_values[func_i][x_value] = y_value



    def Draw_graph(self, x_size: tuple[float, float], y_size: tuple[float, float], point_distance: float) -> None:
        self.Draw_grid_and_axes(x_size, y_size)

        if self.screen is None:
            return
        # Point preparation
        wanted_x_values: list[float] = []
        for x_i in range(int(x_size[0] / point_distance), int(x_size[1] / point_distance) + 1, 1):
            x_value: float = x_i * point_distance
            wanted_x_values.append(x_value)

        for func_i in range(len(self.functions)):
            color: tuple[int, int, int] = self.Get_func_color(func_i)
            x_i = -1
            for x_value in wanted_x_values:
                x_i += 1
                y_value: float | None = self.y_values[func_i].get(x_value, None)
                if y_value is None:
                    continue

                screen_coords: tuple[int, int] = self.XY_to_screen(x_value, y_value, x_size, y_size)
                # if 0 <= screen_coords[0] < self.width and 0 <= screen_coords[1] < self.height:
                #     pygame.draw.circle(self.screen, color, screen_coords, 1)

                if x_i > 0:
                    prev_x_value: float = wanted_x_values[x_i-1]
                    prev_y_value: float | None = self.y_values[func_i].get(prev_x_value, None)
                    if prev_y_value is not None:
                        prev_screen_coords: tuple[int, int] = self.XY_to_screen(prev_x_value, prev_y_value, x_size, y_size)
                        if (0 <= prev_screen_coords[0] < self.width and 0 <= prev_screen_coords[1] < self.height) or (0 <= screen_coords[0] < self.width and 0 <= screen_coords[1] < self.height):
                            pygame.draw.line(self.screen, color, prev_screen_coords, screen_coords, 2)

        self.Draw_legend()

    def Draw_legend(self) -> None:
        if self.screen is None:
            return
        if len(self.functions) == 0:
            return

        if not pygame.font.get_init():
            pygame.font.init()

        font: pygame.font.Font = pygame.font.SysFont(None, 22)
        item_gap: int = 6
        line_height: int = 20
        marker_radius: int = 5
        marker_offset_x: int = 12
        text_offset_x: int = 24
        padding_x: int = 10
        padding_y: int = 8

        legend_texts: list[pygame.Surface] = []
        max_text_width: int = 0
        for func in self.functions:
            text_surface: pygame.Surface = font.render(func, True, (15, 15, 15))
            legend_texts.append(text_surface)
            if text_surface.get_width() > max_text_width:
                max_text_width = text_surface.get_width()

        legend_width: int = padding_x * 2 + text_offset_x + max_text_width
        legend_height: int = padding_y * 2 + len(self.functions) * line_height + (len(self.functions) - 1) * item_gap
        legend_rect: pygame.Rect = pygame.Rect(12, 12, legend_width, legend_height)

        legend_bg: pygame.Surface = pygame.Surface((legend_rect.width, legend_rect.height), pygame.SRCALPHA)
        legend_bg.fill((245, 245, 245, 220))
        pygame.draw.rect(legend_bg, (40, 40, 40), legend_bg.get_rect(), 2, border_radius=8)
        self.screen.blit(legend_bg, legend_rect)

        y_cursor: int = legend_rect.top + padding_y
        for func_i in range(len(self.functions)):
            color: tuple[int, int, int] = self.Get_func_color(func_i)
            marker_center: tuple[int, int] = (legend_rect.left + marker_offset_x, y_cursor + line_height // 2)
            pygame.draw.circle(self.screen, color, marker_center, marker_radius)
            text_pos: tuple[int, int] = (legend_rect.left + text_offset_x, y_cursor)
            self.screen.blit(legend_texts[func_i], text_pos)
            y_cursor += line_height + item_gap

    def Draw_grid_and_axes(self, x_size: tuple[float, float], y_size: tuple[float, float]) -> None:
        if self.screen is None:
            return
        axis_color: tuple[int, int, int] = (0, 0, 0)
        grid_color: tuple[int, int, int] = (225, 225, 225)
        label_color: tuple[int, int, int] = (90, 90, 90)
        font: pygame.font.Font = pygame.font.SysFont(None, 16)

        x_step: float = self.Get_nice_step(x_size[1] - x_size[0])
        y_step: float = self.Get_nice_step(y_size[1] - y_size[0])

        x_zero: int | None = None
        y_zero: int | None = None
        if x_size[0] <= 0 <= x_size[1]:
            x_zero = self.XY_to_screen(0, 0, x_size, y_size)[0]
        if y_size[0] <= 0 <= y_size[1]:
            y_zero = self.XY_to_screen(0, 0, x_size, y_size)[1]

        def first_step_value(min_value: float, step: float) -> float:
            return math.ceil(min_value / step) * step

        def draw_label(value: float, position: tuple[int, int], horizontal: bool) -> None:
            if self.screen is None:
                return
            text_surface: pygame.Surface = font.render(self.Format_axis_value(value), True, label_color)
            text_rect: pygame.Rect = text_surface.get_rect()
            if horizontal:
                text_rect.midtop = position
                text_rect.y += 2
            else:
                text_rect.midright = position
                text_rect.x -= 4
            self.screen.blit(text_surface, text_rect)

        x_tick: float = first_step_value(x_size[0], x_step)
        while x_tick <= x_size[1] + x_step * 0.5:
            screen_x: int = self.XY_to_screen(x_tick, 0, x_size, y_size)[0]
            pygame.draw.line(self.screen, grid_color, (screen_x, 0), (screen_x, self.height), 1)
            label_y: int = (y_zero + 2) if y_zero is not None else self.height - 18
            draw_label(x_tick, (screen_x, label_y), True)
            x_tick += x_step

        y_tick: float = first_step_value(y_size[0], y_step)
        while y_tick <= y_size[1] + y_step * 0.5:
            screen_y: int = self.XY_to_screen(0, y_tick, x_size, y_size)[1]
            pygame.draw.line(self.screen, grid_color, (0, screen_y), (self.width, screen_y), 1)
            label_x: int = (x_zero - 4) if x_zero is not None else 28
            draw_label(y_tick, (label_x, screen_y), False)
            y_tick += y_step

        if x_zero is not None:
            pygame.draw.line(self.screen, axis_color, (x_zero, 0), (x_zero, self.height), 2)
        if y_zero is not None:
            pygame.draw.line(self.screen, axis_color, (0, y_zero), (self.width, y_zero), 2)

    def Get_nice_step(self, span: float) -> float:
        if span <= 0:
            return 1.0

        target_steps: float = 8.0
        raw_step: float = span / target_steps
        power: float = 10 ** math.floor(math.log10(raw_step))
        normalized: float = raw_step / power
        if normalized <= 1:
            nice: float = 1
        elif normalized <= 2:
            nice = 2
        elif normalized <= 5:
            nice = 5
        else:
            nice = 10
        return nice * power

    def Format_axis_value(self, value: float) -> str:
        rounded_value: float = round(value)
        if abs(value - rounded_value) < 1e-9:
            return str(int(rounded_value))
        return f"{value:.6g}"
    


    def Check_for_moving(self) -> None:
        # Moving Logic
        if pygame.key.get_pressed()[pygame.K_LEFT]:
            if not self.moved:
                self.moved = True
                self.x_size = (self.x_size[0] - 0.1 * (self.x_size[1] - self.x_size[0]), self.x_size[1] - 0.1 * (self.x_size[1] - self.x_size[0]))
                self.Prepare_graph_drawing()
        elif pygame.key.get_pressed()[pygame.K_RIGHT]:
            if not self.moved:
                self.moved = True
                self.x_size = (self.x_size[0] + 0.1 * (self.x_size[1] - self.x_size[0]), self.x_size[1] + 0.1 * (self.x_size[1] - self.x_size[0]))
                self.Prepare_graph_drawing()
        elif pygame.key.get_pressed()[pygame.K_UP]:
            if not self.moved:
                self.moved = True
                if pygame.key.get_pressed()[pygame.K_LSHIFT] or pygame.key.get_pressed()[pygame.K_RSHIFT]:
                    self.zoom += 1
                    # Zoom in by reducing the visible span to 50% around the same center.
                    self.y_size = (self.y_size[0] + 0.25 * (self.y_size[1] - self.y_size[0]), self.y_size[1] - 0.25 * (self.y_size[1] - self.y_size[0]))
                    self.x_size = (self.x_size[0] + 0.25 * (self.x_size[1] - self.x_size[0]), self.x_size[1] - 0.25 * (self.x_size[1] - self.x_size[0]))
                    self.Prepare_graph_drawing()
                else:
                    self.y_size = (self.y_size[0] + 0.1 * (self.y_size[1] - self.y_size[0]), self.y_size[1] + 0.1 * (self.y_size[1] - self.y_size[0]))
                    self.Prepare_graph_drawing()
        elif pygame.key.get_pressed()[pygame.K_DOWN]:
            if not self.moved:
                self.moved = True
                if pygame.key.get_pressed()[pygame.K_LSHIFT] or pygame.key.get_pressed()[pygame.K_RSHIFT]:
                    self.zoom -= 1
                    self.y_size = (self.y_size[0] - 0.5 * (self.y_size[1] - self.y_size[0]), self.y_size[1] + 0.5 * (self.y_size[1] - self.y_size[0]))
                    self.x_size = (self.x_size[0] - 0.5 * (self.x_size[1] - self.x_size[0]), self.x_size[1] + 0.5 * (self.x_size[1] - self.x_size[0]))
                    self.Prepare_graph_drawing()
                else:
                    self.y_size = (self.y_size[0] - 0.1 * (self.y_size[1] - self.y_size[0]), self.y_size[1] - 0.1 * (self.y_size[1] - self.y_size[0]))
                    self.Prepare_graph_drawing()
        else:
            self.moved = False






    def Prepare_graph_drawing(self) -> None:
        point_distance: float = 1 / 2 ** self.zoom
        
        wanted_x_values: list[float] = []
        for x_i in range(int(self.x_size[0] / point_distance), int(self.x_size[1] / point_distance) + 1, 1):
            x_value: float = x_i * point_distance
            wanted_x_values.append(x_value)
        
        self.Calculate_points(self.x_size, self.zoom)


    def XY_to_screen(self, x: float, y: float, x_size: tuple[float, float], y_size: tuple[float, float]) -> tuple[int, int]:
        # Convert the mathematical x and y values to screen coordinates
        x_span: float = x_size[1] - x_size[0]
        y_span: float = y_size[1] - y_size[0]
        if abs(x_span) < 1e-12:
            x_span = 1e-12
        if abs(y_span) < 1e-12:
            y_span = 1e-12

        screen_x: int = int((x - x_size[0]) / x_span * self.width)
        screen_y: int = int((y_size[1] - y) / y_span * self.height)
        return (screen_x, screen_y)

    def Draw_calculating_overlay(self) -> None:
        if self.screen is None:
            return

        if not pygame.get_init():
            return

        if not pygame.font.get_init():
            pygame.font.init()

        if self.last_frame is not None:
            self.screen.blit(self.last_frame, (0, 0))

        font: pygame.font.Font = pygame.font.SysFont(None, 28)
        text_surface: pygame.Surface = font.render("Calculating...", True, (20, 20, 20))

        padding_x: int = 18
        padding_y: int = 12
        box_width: int = text_surface.get_width() + 2 * padding_x
        box_height: int = text_surface.get_height() + 2 * padding_y
        box_rect: pygame.Rect = pygame.Rect(0, 0, box_width, box_height)
        box_rect.center = (self.width // 2, self.height // 2)

        box_surface: pygame.Surface = pygame.Surface((box_width, box_height), pygame.SRCALPHA)
        box_surface.fill((245, 245, 245, 235))
        pygame.draw.rect(box_surface, (40, 40, 40), box_surface.get_rect(), 2, border_radius=8)

        self.screen.blit(box_surface, box_rect)
        text_rect: pygame.Rect = text_surface.get_rect(center=box_rect.center)
        self.screen.blit(text_surface, text_rect)
        pygame.display.flip()

    def Get_func_color(self, func_i: int) -> tuple[int, int, int]:
        # Return a color based on the function index
        colors: list[tuple[int, int, int]] = [(200, 0, 0), (0, 200, 0), (0, 0, 200), (200, 100, 0), (200, 0, 200), (0, 200, 200)]
        result: tuple[int, int, int] = colors[func_i % len(colors)]
        if func_i >= len(colors):
            result = (result[0] // (func_i // len(colors)+1), result[1] // (func_i // len(colors)+1), result[2] // (func_i // len(colors)+1)) # Darken the color for higher indices
        return result
        


